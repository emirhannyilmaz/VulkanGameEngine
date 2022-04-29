#include "skybox_renderer.hpp"

SkyboxRenderer::SkyboxRenderer(Renderer* renderer) {
    this->renderer = renderer;

    std::array<VkDescriptorSetLayoutBinding, 1> layoutBindings{};
    layoutBindings[0] = {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr};
    descriptorSetLayout = new DescriptorSetLayout(renderer->device->device, static_cast<uint32_t>(layoutBindings.size()), layoutBindings.data());

    Skybox::CreateDesriptorSetLayout(renderer->device->device);

    std::array<VkDescriptorSetLayout, 2> descriptorSetLayouts{};
    descriptorSetLayouts[0] = descriptorSetLayout->descriptorSetLayout;
    descriptorSetLayouts[1] = Skybox::descriptorSetLayout->descriptorSetLayout;

    graphicsPipeline = new GraphicsPipeline(renderer->device->device, "res/shaders/skybox_shader.vert.spv", "res/shaders/skybox_shader.frag.spv", 0, nullptr, 0, nullptr, static_cast<uint32_t>(descriptorSetLayouts.size()), descriptorSetLayouts.data(), renderer->swapchain->swapchainExtent, renderer->renderPass->renderPass, renderer->device->msaaSamples);
    offScreenGraphicsPipeline = new GraphicsPipeline(renderer->device->device, "res/shaders/skybox_shader.vert.spv", "res/shaders/skybox_shader.frag.spv", 0, nullptr, 0, nullptr, static_cast<uint32_t>(descriptorSetLayouts.size()), descriptorSetLayouts.data(), renderer->swapchain->swapchainExtent, renderer->waterResources->renderPass->renderPass, VK_SAMPLE_COUNT_1_BIT);

    std::array<VkDescriptorPoolSize, 1> poolSizes{};
    poolSizes[0] = {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_FRAMES_IN_FLIGHT};
    descriptorPool = new DescriptorPool(renderer->device->device, static_cast<uint32_t>(poolSizes.size()), poolSizes.data(), MAX_FRAMES_IN_FLIGHT);

    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout->descriptorSetLayout);
    descriptorSets = new DescriptorSets(renderer->device->device, descriptorPool->descriptorPool, layouts.data(), MAX_FRAMES_IN_FLIGHT);

    vertexUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vertexUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(SkyboxRendererVertexUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 0, 0, 1, vertexUniformBuffers[i]->buffer, sizeof(SkyboxRendererVertexUniformBufferObject));
    }
}

SkyboxRenderer::~SkyboxRenderer() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        delete vertexUniformBuffers[i];
    }
    vertexUniformBuffers.clear();

    delete descriptorPool;
    delete offScreenGraphicsPipeline;
    delete graphicsPipeline;
    Skybox::DeleteDesriptorSetLayout();
    delete descriptorSetLayout;
}

void SkyboxRenderer::render(Skybox* skybox, Camera* camera, CommandBuffers* commandBuffers, bool onScreen) {
    updateDescriptorSetResources(camera);

    VkCommandBuffer commandBuffer = commandBuffers->commandBuffers[renderer->currentFrame];
    GraphicsPipeline* graphicsPipeline = onScreen ? graphicsPipeline : offScreenGraphicsPipeline;

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->graphicsPipeline);

    std::array<VkDescriptorSet, 2> descriptorSetsToBind{};
    descriptorSetsToBind[0] = descriptorSets->descriptorSets[renderer->currentFrame];
    descriptorSetsToBind[1] = skybox->descriptorSets->descriptorSets[renderer->currentFrame];
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->pipelineLayout, 0, static_cast<uint32_t>(descriptorSetsToBind.size()), descriptorSetsToBind.data(), 0, nullptr);

    vkCmdDraw(commandBuffer, 36, 1, 0, 0);
}

void SkyboxRenderer::updateDescriptorSetResources(Camera* camera) {
    SkyboxRendererVertexUniformBufferObject vertexUbo{};
    vertexUbo.viewMatrix = camera->createViewMatrix();
    vertexUbo.viewMatrix[3][0] = 0.0f;
    vertexUbo.viewMatrix[3][1] = 0.0f;
    vertexUbo.viewMatrix[3][2] = 0.0f;
    vertexUbo.projectionMatrix = camera->createProjectionMatrix();
    void* vubData;
    vkMapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory, 0, sizeof(vertexUbo), 0, &vubData);
    memcpy(vubData, &vertexUbo, sizeof(vertexUbo));
    vkUnmapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory);
}
