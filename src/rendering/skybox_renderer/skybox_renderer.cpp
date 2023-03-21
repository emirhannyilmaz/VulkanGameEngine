#include "skybox_renderer.hpp"

SkyboxRenderer::SkyboxRenderer(Renderer* renderer) {
    this->renderer = renderer;

    std::array<VkDescriptorSetLayoutBinding, 2> layoutBindings{};
    layoutBindings[0] = {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr};
    layoutBindings[1] = {1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
    descriptorSetLayout = new DescriptorSetLayout(renderer->device->device, static_cast<uint32_t>(layoutBindings.size()), layoutBindings.data());

    Skybox::CreateDesriptorSetLayout(renderer->device->device);

    CreateGraphicsPipelines();

    std::array<VkDescriptorPoolSize, 1> poolSizes{};
    poolSizes[0] = {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2 * MAX_FRAMES_IN_FLIGHT};
    descriptorPool = new DescriptorPool(renderer->device->device, static_cast<uint32_t>(poolSizes.size()), poolSizes.data(), MAX_FRAMES_IN_FLIGHT);

    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout->descriptorSetLayout);
    descriptorSets = new DescriptorSets(renderer->device->device, descriptorPool->descriptorPool, layouts.data(), MAX_FRAMES_IN_FLIGHT);

    vertexUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    fragmentUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vertexUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(SkyboxRendererVertexUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 0, 0, 1, vertexUniformBuffers[i]->buffer, sizeof(SkyboxRendererVertexUniformBufferObject));

        fragmentUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(SkyboxRendererFragmentUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 1, 0, 1, fragmentUniformBuffers[i]->buffer, sizeof(SkyboxRendererFragmentUniformBufferObject));
    }
}

SkyboxRenderer::~SkyboxRenderer() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        delete vertexUniformBuffers[i];
        delete fragmentUniformBuffers[i];
    }
    vertexUniformBuffers.clear();
    fragmentUniformBuffers.clear();

    delete descriptorPool;
    DeleteGraphicsPipelines();
    Skybox::DeleteDesriptorSetLayout();
    delete descriptorSetLayout;
}

void SkyboxRenderer::CreateGraphicsPipelines() {
    std::array<VkDescriptorSetLayout, 2> descriptorSetLayouts{};
    descriptorSetLayouts[0] = descriptorSetLayout->descriptorSetLayout;
    descriptorSetLayouts[1] = Skybox::descriptorSetLayout->descriptorSetLayout;

    std::array<VkPushConstantRange, 1> pushConstantRanges{};
    pushConstantRanges[0] = {VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(SkyboxRendererVertexPushConstants)};

    graphicsPipeline = new GraphicsPipeline(renderer->device->device, "res/shaders/skybox_shader.vert.spv", "res/shaders/skybox_shader.frag.spv", 0, nullptr, 0, nullptr, static_cast<uint32_t>(descriptorSetLayouts.size()), descriptorSetLayouts.data(), static_cast<uint32_t>(pushConstantRanges.size()), pushConstantRanges.data(), renderer->swapchain->swapchainExtent, renderer->renderPass->renderPass, renderer->device->msaaSamples, VK_TRUE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA);
    graphicsPipelineForWater = new GraphicsPipeline(renderer->device->device, "res/shaders/skybox_shader.vert.spv", "res/shaders/skybox_shader.frag.spv", 0, nullptr, 0, nullptr, static_cast<uint32_t>(descriptorSetLayouts.size()), descriptorSetLayouts.data(), static_cast<uint32_t>(pushConstantRanges.size()), pushConstantRanges.data(), renderer->swapchain->swapchainExtent, renderer->waterResources->renderPass->renderPass, VK_SAMPLE_COUNT_1_BIT, VK_TRUE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA);
    graphicsPipelineForGaussianBlur = new GraphicsPipeline(renderer->device->device, "res/shaders/skybox_shader.vert.spv", "res/shaders/skybox_shader.frag.spv", 0, nullptr, 0, nullptr, static_cast<uint32_t>(descriptorSetLayouts.size()), descriptorSetLayouts.data(), static_cast<uint32_t>(pushConstantRanges.size()), pushConstantRanges.data(), renderer->swapchain->swapchainExtent, renderer->gaussianBlurResources->rawRenderPass->renderPass, VK_SAMPLE_COUNT_1_BIT, VK_TRUE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA);
}

void SkyboxRenderer::DeleteGraphicsPipelines() {
    delete graphicsPipelineForGaussianBlur;
    delete graphicsPipelineForWater;
    delete graphicsPipeline;
}

void SkyboxRenderer::render(Skybox* skybox, PerspectiveCamera* perspectiveCamera, glm::vec4 clipPlane, CommandBuffers* commandBuffers, GraphicsPipeline* graphicsPipeline) {
    updateDescriptorSetResources(perspectiveCamera);
    updatePushConstants(perspectiveCamera, clipPlane);

    VkCommandBuffer commandBuffer = commandBuffers->commandBuffers[renderer->currentFrame];

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->graphicsPipeline);
    vkCmdPushConstants(commandBuffer, graphicsPipeline->pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(SkyboxRendererVertexPushConstants), &vertexPushConstants);

    std::array<VkDescriptorSet, 2> descriptorSetsToBind{};
    descriptorSetsToBind[0] = descriptorSets->descriptorSets[renderer->currentFrame];
    descriptorSetsToBind[1] = skybox->descriptorSets->descriptorSets[renderer->currentFrame];
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->pipelineLayout, 0, static_cast<uint32_t>(descriptorSetsToBind.size()), descriptorSetsToBind.data(), 0, nullptr);

    vkCmdDraw(commandBuffer, 36, 1, 0, 0);
}

void SkyboxRenderer::updateDescriptorSetResources(PerspectiveCamera* perspectiveCamera) {
    SkyboxRendererVertexUniformBufferObject vertexUbo{};
    vertexUbo.projectionMatrix = perspectiveCamera->createProjectionMatrix();
    void* vubData;
    vkMapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory, 0, sizeof(vertexUbo), 0, &vubData);
    memcpy(vubData, &vertexUbo, sizeof(vertexUbo));
    vkUnmapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory);

    SkyboxRendererFragmentUniformBufferObject fragmentUbo{};
    fragmentUbo.fogColor = FOG_COLOR;
    void* fubData;
    vkMapMemory(renderer->device->device, fragmentUniformBuffers[renderer->currentFrame]->bufferMemory, 0, sizeof(fragmentUbo), 0, &fubData);
    memcpy(fubData, &fragmentUbo, sizeof(fragmentUbo));
    vkUnmapMemory(renderer->device->device, fragmentUniformBuffers[renderer->currentFrame]->bufferMemory);
}

void SkyboxRenderer::updatePushConstants(PerspectiveCamera* perspectiveCamera, glm::vec4 clipPlane) {
    vertexPushConstants.viewMatrix = perspectiveCamera->createViewMatrix();
    vertexPushConstants.viewMatrix[3][0] = 0.0f;
    vertexPushConstants.viewMatrix[3][1] = 0.0f;
    vertexPushConstants.viewMatrix[3][2] = 0.0f;
    vertexPushConstants.clipPlane = clipPlane;
}
