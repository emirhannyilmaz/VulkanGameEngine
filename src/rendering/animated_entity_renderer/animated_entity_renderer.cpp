#include "animated_entity_renderer.hpp"

AnimatedEntityRenderer::AnimatedEntityRenderer(Renderer* renderer) {
    this->renderer = renderer;

    std::array<VkDescriptorSetLayoutBinding, 2> layoutBindings{};
    layoutBindings[0] = {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr};
    layoutBindings[1] = {1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
    descriptorSetLayout = new DescriptorSetLayout(renderer->device->device, static_cast<uint32_t>(layoutBindings.size()), layoutBindings.data());

    AnimatedEntity::CreateDesriptorSetLayout(renderer->device->device);

    CreateGraphicsPipelines();

    std::array<VkDescriptorPoolSize, 1> poolSizes{};
    poolSizes[0] = {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2 * MAX_FRAMES_IN_FLIGHT};
    descriptorPool = new DescriptorPool(renderer->device->device, static_cast<uint32_t>(poolSizes.size()), poolSizes.data(), MAX_FRAMES_IN_FLIGHT);

    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout->descriptorSetLayout);
    descriptorSets = new DescriptorSets(renderer->device->device, descriptorPool->descriptorPool, layouts.data(), MAX_FRAMES_IN_FLIGHT);

    vertexUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    fragmentUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vertexUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(AnimatedEntityRendererVertexUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 0, 0, 1, vertexUniformBuffers[i]->buffer, sizeof(AnimatedEntityRendererVertexUniformBufferObject));

        fragmentUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(AnimatedEntityRendererFragmentUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 1, 0, 1, fragmentUniformBuffers[i]->buffer, sizeof(AnimatedEntityRendererFragmentUniformBufferObject));
    }
}

AnimatedEntityRenderer::~AnimatedEntityRenderer() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        delete vertexUniformBuffers[i];
        delete fragmentUniformBuffers[i];
    }
    vertexUniformBuffers.clear();
    fragmentUniformBuffers.clear();

    delete descriptorPool;
    DeleteGraphicsPipelines();
    AnimatedEntity::DeleteDesriptorSetLayout();
    delete descriptorSetLayout;
}

void AnimatedEntityRenderer::CreateGraphicsPipelines() {
    auto bindingDescription = AnimatedEntityVertex::getBindingDescription();
    auto attributeDescriptions = AnimatedEntityVertex::getAttributeDescriptions();

    std::array<VkDescriptorSetLayout, 2> descriptorSetLayouts{};
    descriptorSetLayouts[0] = descriptorSetLayout->descriptorSetLayout;
    descriptorSetLayouts[1] = AnimatedEntity::descriptorSetLayout->descriptorSetLayout;

    std::array<VkPushConstantRange, 1> pushConstantRanges{};
    pushConstantRanges[0] = {VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(AnimatedEntityRendererVertexPushConstants)};

    graphicsPipeline = new GraphicsPipeline(renderer->device->device, "res/shaders/animated_entity_shader.vert.spv", "res/shaders/animated_entity_shader.frag.spv", 1, &bindingDescription, static_cast<uint32_t>(attributeDescriptions.size()), attributeDescriptions.data(), static_cast<uint32_t>(descriptorSetLayouts.size()), descriptorSetLayouts.data(), static_cast<uint32_t>(pushConstantRanges.size()), pushConstantRanges.data(), renderer->swapchain->swapchainExtent, renderer->renderPass->renderPass, renderer->device->msaaSamples, VK_TRUE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA);
    graphicsPipelineForWater = new GraphicsPipeline(renderer->device->device, "res/shaders/animated_entity_shader.vert.spv", "res/shaders/animated_entity_shader.frag.spv", 1, &bindingDescription, static_cast<uint32_t>(attributeDescriptions.size()), attributeDescriptions.data(), static_cast<uint32_t>(descriptorSetLayouts.size()), descriptorSetLayouts.data(), static_cast<uint32_t>(pushConstantRanges.size()), pushConstantRanges.data(), renderer->swapchain->swapchainExtent, renderer->waterResources->renderPass->renderPass, VK_SAMPLE_COUNT_1_BIT, VK_TRUE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA);
    graphicsPipelineForGaussianBlur = new GraphicsPipeline(renderer->device->device, "res/shaders/animated_entity_shader.vert.spv", "res/shaders/animated_entity_shader.frag.spv", 1, &bindingDescription, static_cast<uint32_t>(attributeDescriptions.size()), attributeDescriptions.data(), static_cast<uint32_t>(descriptorSetLayouts.size()), descriptorSetLayouts.data(), static_cast<uint32_t>(pushConstantRanges.size()), pushConstantRanges.data(), renderer->swapchain->swapchainExtent, renderer->gaussianBlurResources->rawRenderPass->renderPass, VK_SAMPLE_COUNT_1_BIT, VK_TRUE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA);
}

void AnimatedEntityRenderer::DeleteGraphicsPipelines() {
    delete graphicsPipelineForGaussianBlur;
    delete graphicsPipelineForWater;
    delete graphicsPipeline;
}

void AnimatedEntityRenderer::render(std::vector<AnimatedEntity*> animatedEntities, Light* light, PerspectiveCamera* perspectiveCamera, glm::vec4 clipPlane, CommandBuffers* commandBuffers, GraphicsPipeline* graphicsPipeline) {
    updateDescriptorSetResources(light, perspectiveCamera);
    updatePushConstants(perspectiveCamera, clipPlane);

    VkCommandBuffer commandBuffer = commandBuffers->commandBuffers[renderer->currentFrame];

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->graphicsPipeline);
    vkCmdPushConstants(commandBuffer, graphicsPipeline->pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(AnimatedEntityRendererVertexPushConstants), &vertexPushConstants);

    std::array<VkDescriptorSet, 2> descriptorSetsToBind{};
    descriptorSetsToBind[0] = descriptorSets->descriptorSets[renderer->currentFrame];
    for (AnimatedEntity* animatedEntity : animatedEntities) {
        animatedEntity->updateDescriptorSetResources();

        VkDeviceSize offsets = 0;
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, &animatedEntity->mesh->vertexBuffer->buffer, &offsets);
        vkCmdBindIndexBuffer(commandBuffer, animatedEntity->mesh->indexBuffer->buffer, 0, VK_INDEX_TYPE_UINT32);
        descriptorSetsToBind[1] = animatedEntity->descriptorSets->descriptorSets[renderer->currentFrame];
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->pipelineLayout, 0, static_cast<uint32_t>(descriptorSetsToBind.size()), descriptorSetsToBind.data(), 0, nullptr);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(animatedEntity->mesh->indicesSize), 1, 0, 0, 0);
    }
}

void AnimatedEntityRenderer::updateDescriptorSetResources(Light* light, PerspectiveCamera* perspectiveCamera) {
    AnimatedEntityRendererVertexUniformBufferObject vertexUbo{};
    vertexUbo.projectionMatrix = perspectiveCamera->createProjectionMatrix();
    vertexUbo.lightPosition = light->position;
    void* vubData;
    vkMapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory, 0, sizeof(vertexUbo), 0, &vubData);
    memcpy(vubData, &vertexUbo, sizeof(vertexUbo));
    vkUnmapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory);

    AnimatedEntityRendererFragmentUniformBufferObject fragmentUbo{};
    fragmentUbo.lightColor = light->color;
    fragmentUbo.fogColor = FOG_COLOR;
    void* fubData;
    vkMapMemory(renderer->device->device, fragmentUniformBuffers[renderer->currentFrame]->bufferMemory, 0, sizeof(fragmentUbo), 0, &fubData);
    memcpy(fubData, &fragmentUbo, sizeof(fragmentUbo));
    vkUnmapMemory(renderer->device->device, fragmentUniformBuffers[renderer->currentFrame]->bufferMemory);
}

void AnimatedEntityRenderer::updatePushConstants(PerspectiveCamera* perspectiveCamera, glm::vec4 clipPlane) {
    vertexPushConstants.viewMatrix = perspectiveCamera->createViewMatrix();
    vertexPushConstants.clipPlane = clipPlane;
}
