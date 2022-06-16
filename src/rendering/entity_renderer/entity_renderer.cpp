#include "entity_renderer.hpp"

EntityRenderer::EntityRenderer(Renderer* renderer) {
    this->renderer = renderer;

    std::array<VkDescriptorSetLayoutBinding, 2> layoutBindings{};
    layoutBindings[0] = {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr};
    layoutBindings[1] = {1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
    descriptorSetLayout = new DescriptorSetLayout(renderer->device->device, static_cast<uint32_t>(layoutBindings.size()), layoutBindings.data());

    Entity::CreateDesriptorSetLayout(renderer->device->device);

    CreateGraphicsPipelines();

    std::array<VkDescriptorPoolSize, 1> poolSizes{};
    poolSizes[0] = {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2 * MAX_FRAMES_IN_FLIGHT};
    descriptorPool = new DescriptorPool(renderer->device->device, static_cast<uint32_t>(poolSizes.size()), poolSizes.data(), MAX_FRAMES_IN_FLIGHT);

    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout->descriptorSetLayout);
    descriptorSets = new DescriptorSets(renderer->device->device, descriptorPool->descriptorPool, layouts.data(), MAX_FRAMES_IN_FLIGHT);

    vertexUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    fragmentUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vertexUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(EntityRendererVertexUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 0, 0, 1, vertexUniformBuffers[i]->buffer, sizeof(EntityRendererVertexUniformBufferObject));

        fragmentUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(EntityRendererFragmentUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 1, 0, 1, fragmentUniformBuffers[i]->buffer, sizeof(EntityRendererFragmentUniformBufferObject));
    }
}

EntityRenderer::~EntityRenderer() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        delete vertexUniformBuffers[i];
        delete fragmentUniformBuffers[i];
    }
    vertexUniformBuffers.clear();
    fragmentUniformBuffers.clear();

    delete descriptorPool;
    Entity::DeleteDesriptorSetLayout();
    delete descriptorSetLayout;
}

void EntityRenderer::CreateGraphicsPipelines() {
    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    std::array<VkDescriptorSetLayout, 2> descriptorSetLayouts{};
    descriptorSetLayouts[0] = descriptorSetLayout->descriptorSetLayout;
    descriptorSetLayouts[1] = Entity::descriptorSetLayout->descriptorSetLayout;

    std::array<VkPushConstantRange, 1> pushConstantRanges{};
    pushConstantRanges[0] = {VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(EntityRendererVertexPushConstants)};

    graphicsPipeline = new GraphicsPipeline(renderer->device->device, "res/shaders/entity_shader.vert.spv", "res/shaders/entity_shader.frag.spv", 1, &bindingDescription, static_cast<uint32_t>(attributeDescriptions.size()), attributeDescriptions.data(), static_cast<uint32_t>(descriptorSetLayouts.size()), descriptorSetLayouts.data(), static_cast<uint32_t>(pushConstantRanges.size()), pushConstantRanges.data(), renderer->swapchain->swapchainExtent, renderer->renderPass->renderPass, renderer->device->msaaSamples);
    offScreenGraphicsPipeline = new GraphicsPipeline(renderer->device->device, "res/shaders/entity_shader.vert.spv", "res/shaders/entity_shader.frag.spv", 1, &bindingDescription, static_cast<uint32_t>(attributeDescriptions.size()), attributeDescriptions.data(), static_cast<uint32_t>(descriptorSetLayouts.size()), descriptorSetLayouts.data(), static_cast<uint32_t>(pushConstantRanges.size()), pushConstantRanges.data(), renderer->swapchain->swapchainExtent, renderer->waterResources->renderPass->renderPass, VK_SAMPLE_COUNT_1_BIT);
}

void EntityRenderer::DeleteGraphicsPipelines() {
    delete graphicsPipeline;
    delete offScreenGraphicsPipeline;
}

void EntityRenderer::render(std::vector<Entity*> entities, Light* light, PerspectiveCamera* perspectiveCamera, glm::vec3 fogColor, glm::vec4 clipPlane, CommandBuffers* commandBuffers, bool onScreen) {
    updateDescriptorSetResources(light, perspectiveCamera, fogColor);
    updatePushConstants(perspectiveCamera, clipPlane);

    VkCommandBuffer commandBuffer = commandBuffers->commandBuffers[renderer->currentFrame];
    GraphicsPipeline* gp = onScreen ? graphicsPipeline : offScreenGraphicsPipeline;

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, gp->graphicsPipeline);
    vkCmdPushConstants(commandBuffer, gp->pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(EntityRendererVertexPushConstants), &vertexPushConstants);

    std::array<VkDescriptorSet, 2> descriptorSetsToBind{};
    descriptorSetsToBind[0] = descriptorSets->descriptorSets[renderer->currentFrame];
    for (Entity* entity : entities) {
        entity->updateDescriptorSetResources();

        VkDeviceSize offsets = 0;
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, &entity->mesh->vertexBuffer->buffer, &offsets);
        vkCmdBindIndexBuffer(commandBuffer, entity->mesh->indexBuffer->buffer, 0, VK_INDEX_TYPE_UINT32);
        descriptorSetsToBind[1] = entity->descriptorSets->descriptorSets[renderer->currentFrame];
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, gp->pipelineLayout, 0, static_cast<uint32_t>(descriptorSetsToBind.size()), descriptorSetsToBind.data(), 0, nullptr);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(entity->mesh->indicesSize), 1, 0, 0, 0);
    }
}

void EntityRenderer::updateDescriptorSetResources(Light* light, PerspectiveCamera* perspectiveCamera, glm::vec3 fogColor) {
    EntityRendererVertexUniformBufferObject vertexUbo{};
    vertexUbo.projectionMatrix = perspectiveCamera->createProjectionMatrix();
    vertexUbo.lightPosition = light->position;
    void* vubData;
    vkMapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory, 0, sizeof(vertexUbo), 0, &vubData);
    memcpy(vubData, &vertexUbo, sizeof(vertexUbo));
    vkUnmapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory);

    EntityRendererFragmentUniformBufferObject fragmentUbo{};
    fragmentUbo.lightColor = light->color;
    fragmentUbo.fogColor = fogColor;
    void* fubData;
    vkMapMemory(renderer->device->device, fragmentUniformBuffers[renderer->currentFrame]->bufferMemory, 0, sizeof(fragmentUbo), 0, &fubData);
    memcpy(fubData, &fragmentUbo, sizeof(fragmentUbo));
    vkUnmapMemory(renderer->device->device, fragmentUniformBuffers[renderer->currentFrame]->bufferMemory);
}

void EntityRenderer::updatePushConstants(PerspectiveCamera* perspectiveCamera, glm::vec4 clipPlane) {
    vertexPushConstants.viewMatrix = perspectiveCamera->createViewMatrix();
    vertexPushConstants.clipPlane = clipPlane;
}
