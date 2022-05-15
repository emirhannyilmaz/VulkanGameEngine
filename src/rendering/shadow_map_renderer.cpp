#include "shadow_map_renderer.hpp"

ShadowMapRenderer::ShadowMapRenderer(Renderer* renderer) {
    this->renderer = renderer;

    std::array<VkDescriptorSetLayoutBinding, 7> layoutBindings{};
    layoutBindings[0] = {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr};
    descriptorSetLayout = new DescriptorSetLayout(renderer->device->device, static_cast<uint32_t>(layoutBindings.size()), layoutBindings.data());

    CreateGraphicsPipeline();

    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0] = {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_FRAMES_IN_FLIGHT};
    descriptorPool = new DescriptorPool(renderer->device->device, static_cast<uint32_t>(poolSizes.size()), poolSizes.data(), MAX_FRAMES_IN_FLIGHT);

    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout->descriptorSetLayout);
    descriptorSets = new DescriptorSets(renderer->device->device, descriptorPool->descriptorPool, layouts.data(), MAX_FRAMES_IN_FLIGHT);

    vertexUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vertexUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(ShadowMapRendererVertexUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 0, 0, 1, vertexUniformBuffers[i]->buffer, sizeof(ShadowMapRendererVertexUniformBufferObject));
    }
}

ShadowMapRenderer::~ShadowMapRenderer() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        delete vertexUniformBuffers[i];
    }
    vertexUniformBuffers.clear();

    delete descriptorPool;
    delete descriptorSetLayout;
}

void ShadowMapRenderer::CreateGraphicsPipeline() {
    graphicsPipeline = new GraphicsPipeline(renderer->device->device, "res/shaders/shadow_map_shader.vert.spv", "res/shaders/shadow_map_shader.frag.spv", 0, nullptr, 0, nullptr, 1, &descriptorSetLayout->descriptorSetLayout, 0, nullptr, renderer->swapchain->swapchainExtent, renderer->shadowMapResources->renderPass->renderPass, VK_SAMPLE_COUNT_1_BIT);
}

void ShadowMapRenderer::DeleteGraphicsPipeline() {
    delete graphicsPipeline;
}

void ShadowMapRenderer::render(std::vector<Entity*> entities, Light* light, CommandBuffers* commandBuffers) {
    updateDescriptorSetResources(light);

    VkCommandBuffer commandBuffer = commandBuffers->commandBuffers[renderer->currentFrame];

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->graphicsPipeline);
    for (Entity* entity : entities) {
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->pipelineLayout, 0, 1, &descriptorSets->descriptorSets[renderer->currentFrame], 0, nullptr);
        vkCmdDraw(commandBuffer, 6, 1, 0, 0);
    }
}

void ShadowMapRenderer::updateDescriptorSetResources(Light* light) {
    ShadowMapRendererVertexUniformBufferObject vertexUbo{};
    void* vubData;
    vkMapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory, 0, sizeof(vertexUbo), 0, &vubData);
    memcpy(vubData, &vertexUbo, sizeof(vertexUbo));
    vkUnmapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory);
}
