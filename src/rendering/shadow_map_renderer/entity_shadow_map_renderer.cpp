#include "entity_shadow_map_renderer.hpp"

EntityShadowMapRenderer::EntityShadowMapRenderer(Renderer* renderer) {
    this->renderer = renderer;

    std::array<VkDescriptorSetLayoutBinding, 1> layoutBindings{};
    layoutBindings[0] = {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr};
    descriptorSetLayout = new DescriptorSetLayout(renderer->device->device, static_cast<uint32_t>(layoutBindings.size()), layoutBindings.data());

    CreateGraphicsPipeline();

    std::array<VkDescriptorPoolSize, 1> poolSizes{};
    poolSizes[0] = {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_FRAMES_IN_FLIGHT};
    descriptorPool = new DescriptorPool(renderer->device->device, static_cast<uint32_t>(poolSizes.size()), poolSizes.data(), MAX_FRAMES_IN_FLIGHT);

    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout->descriptorSetLayout);
    descriptorSets = new DescriptorSets(renderer->device->device, descriptorPool->descriptorPool, layouts.data(), MAX_FRAMES_IN_FLIGHT);

    vertexUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vertexUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(EntityShadowMapRendererVertexUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 0, 0, 1, vertexUniformBuffers[i]->buffer, sizeof(EntityShadowMapRendererVertexUniformBufferObject));
    }
}

EntityShadowMapRenderer::~EntityShadowMapRenderer() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        delete vertexUniformBuffers[i];
    }
    vertexUniformBuffers.clear();

    delete descriptorPool;
    DeleteGraphicsPipeline();
    delete descriptorSetLayout;
}

void EntityShadowMapRenderer::CreateGraphicsPipeline() {
    auto bindingDescription = EntityVertex::getBindingDescription();
    auto attributeDescriptions = EntityVertex::getAttributeDescriptions();

    std::array<VkDescriptorSetLayout, 2> descriptorSetLayouts{};
    descriptorSetLayouts[0] = descriptorSetLayout->descriptorSetLayout;
    descriptorSetLayouts[1] = Entity::descriptorSetLayout->descriptorSetLayout;

    graphicsPipeline = new GraphicsPipeline(renderer->device->device, "res/shaders/entity_shadow_map_shader.vert.spv", "res/shaders/entity_shadow_map_shader.frag.spv", 1, &bindingDescription, static_cast<uint32_t>(attributeDescriptions.size()), attributeDescriptions.data(), static_cast<uint32_t>(descriptorSetLayouts.size()), descriptorSetLayouts.data(), 0, nullptr, renderer->shadowMapExtent, renderer->shadowMapResources->renderPass->renderPass, VK_SAMPLE_COUNT_1_BIT, VK_TRUE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA);
}

void EntityShadowMapRenderer::DeleteGraphicsPipeline() {
    delete graphicsPipeline;
}

void EntityShadowMapRenderer::render(std::vector<Entity*> entities, Light* light, PerspectiveCamera* perspectiveCamera, OrthographicCamera* orthographicCamera, CommandBuffers* commandBuffers) {
    updateDescriptorSetResources(light, perspectiveCamera, orthographicCamera);

    VkCommandBuffer commandBuffer = commandBuffers->commandBuffers[renderer->currentFrame];

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->graphicsPipeline);

    std::array<VkDescriptorSet, 2> descriptorSetsToBind{};
    descriptorSetsToBind[0] = descriptorSets->descriptorSets[renderer->currentFrame];
    for (Entity* entity : entities) {
        entity->updateDescriptorSetResources();

        VkDeviceSize offsets = 0;
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, &entity->mesh->vertexBuffer->buffer, &offsets);
        vkCmdBindIndexBuffer(commandBuffer, entity->mesh->indexBuffer->buffer, 0, VK_INDEX_TYPE_UINT32);
        descriptorSetsToBind[1] = entity->descriptorSets->descriptorSets[renderer->currentFrame];
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->pipelineLayout, 0, static_cast<uint32_t>(descriptorSetsToBind.size()), descriptorSetsToBind.data(), 0, nullptr);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(entity->mesh->indicesSize), 1, 0, 0, 0);
    }
}

void EntityShadowMapRenderer::updateDescriptorSetResources(Light* light, PerspectiveCamera* perspectiveCamera, OrthographicCamera* orthographicCamera) {
    EntityShadowMapRendererVertexUniformBufferObject vertexUbo{};
    vertexUbo.viewMatrix = light->createViewMatrix(perspectiveCamera->position, orthographicCamera->getCenterOfZ());
    vertexUbo.projectionMatrix = orthographicCamera->createProjectionMatrix();
    void* vubData;
    vkMapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory, 0, sizeof(vertexUbo), 0, &vubData);
    memcpy(vubData, &vertexUbo, sizeof(vertexUbo));
    vkUnmapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory);
}
