#include "water_renderer.hpp"

WaterRenderer::WaterRenderer(Renderer* renderer) {
    this->renderer = renderer;

    std::array<VkDescriptorSetLayoutBinding, 7> layoutBindings{};
    layoutBindings[0] = {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr};
    layoutBindings[1] = {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
    layoutBindings[2] = {2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
    layoutBindings[3] = {3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
    layoutBindings[4] = {4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
    layoutBindings[5] = {5, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
    layoutBindings[6] = {6, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
    descriptorSetLayout = new DescriptorSetLayout(renderer->device->device, static_cast<uint32_t>(layoutBindings.size()), layoutBindings.data());

    WaterTile::CreateDesriptorSetLayout(renderer->device->device);

    CreateGraphicsPipeline();

    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0] = {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2 * MAX_FRAMES_IN_FLIGHT};
    poolSizes[1] = {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 5 * MAX_FRAMES_IN_FLIGHT};
    descriptorPool = new DescriptorPool(renderer->device->device, static_cast<uint32_t>(poolSizes.size()), poolSizes.data(), MAX_FRAMES_IN_FLIGHT);

    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout->descriptorSetLayout);
    descriptorSets = new DescriptorSets(renderer->device->device, descriptorPool->descriptorPool, layouts.data(), MAX_FRAMES_IN_FLIGHT);

    dudvMap = new Texture("res/textures/water_dudv_map.png", 0, 0, renderer);
    normalMap = new Texture("res/textures/water_normal_map.png", 0, 0, renderer);

    vertexUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    fragmentUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vertexUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(WaterRendererVertexUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 0, 0, 1, vertexUniformBuffers[i]->buffer, sizeof(WaterRendererVertexUniformBufferObject));

        fragmentUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(WaterRendererFragmentUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 6, 0, 1, fragmentUniformBuffers[i]->buffer, sizeof(WaterRendererFragmentUniformBufferObject));
        
        updateDescriptorSetImageInfos();

        descriptorSets->updateImageInfo(i, 3, 0, 1, dudvMap->image->imageView, dudvMap->sampler->sampler);
        descriptorSets->updateImageInfo(i, 4, 0, 1, normalMap->image->imageView, normalMap->sampler->sampler);
    }
}

WaterRenderer::~WaterRenderer() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        delete vertexUniformBuffers[i];
        delete fragmentUniformBuffers[i];
    }
    vertexUniformBuffers.clear();
    fragmentUniformBuffers.clear();

    delete normalMap;
    delete dudvMap;
    delete descriptorPool;
    DeleteGraphicsPipeline();
    WaterTile::DeleteDesriptorSetLayout();
    delete descriptorSetLayout;
}

void WaterRenderer::CreateGraphicsPipeline() {
    auto bindingDescription = WaterVertex::getBindingDescription();
    auto attributeDescriptions = WaterVertex::getAttributeDescriptions();

    std::array<VkDescriptorSetLayout, 2> descriptorSetLayouts{};
    descriptorSetLayouts[0] = descriptorSetLayout->descriptorSetLayout;
    descriptorSetLayouts[1] = WaterTile::descriptorSetLayout->descriptorSetLayout;

    graphicsPipeline = new GraphicsPipeline(renderer->device->device, "res/shaders/water_shader.vert.spv", "res/shaders/water_shader.frag.spv", 1, &bindingDescription, static_cast<uint32_t>(attributeDescriptions.size()), attributeDescriptions.data(), static_cast<uint32_t>(descriptorSetLayouts.size()), descriptorSetLayouts.data(), 0, nullptr, renderer->swapchain->swapchainExtent, renderer->renderPass->renderPass, renderer->device->msaaSamples, VK_TRUE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA);
}

void WaterRenderer::DeleteGraphicsPipeline() {
    delete graphicsPipeline;
}

void WaterRenderer::render(std::vector<WaterTile*> waterTiles, PerspectiveCamera* perspectiveCamera, Light* light, CommandBuffers* commandBuffers) {
    updateDescriptorSetResources(perspectiveCamera, light);

    VkCommandBuffer commandBuffer = commandBuffers->commandBuffers[renderer->currentFrame];

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->graphicsPipeline);

    std::array<VkDescriptorSet, 2> descriptorSetsToBind{};
    descriptorSetsToBind[0] = descriptorSets->descriptorSets[renderer->currentFrame];
    for (WaterTile* waterTile : waterTiles) {
        waterTile->updateDescriptorSetResources();

        VkDeviceSize offsets = 0;
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, &waterTile->mesh->vertexBuffer->buffer, &offsets);
        vkCmdBindIndexBuffer(commandBuffer, waterTile->mesh->indexBuffer->buffer, 0, VK_INDEX_TYPE_UINT32);
        descriptorSetsToBind[1] = waterTile->descriptorSets->descriptorSets[renderer->currentFrame];
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->pipelineLayout, 0, static_cast<uint32_t>(descriptorSetsToBind.size()), descriptorSetsToBind.data(), 0, nullptr);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(waterTile->mesh->indicesSize), 1, 0, 0, 0);
    }
}

void WaterRenderer::updateDescriptorSetImageInfos() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        descriptorSets->updateImageInfo(i, 1, 0, 1, renderer->waterResources->reflectionColorResources->image->imageView, renderer->waterResources->sampler->sampler);
        descriptorSets->updateImageInfo(i, 2, 0, 1, renderer->waterResources->refractionColorResources->image->imageView, renderer->waterResources->sampler->sampler);
        descriptorSets->updateImageInfo(i, 5, 0, 1, renderer->waterResources->refractionDepthResources->image->imageView, renderer->waterResources->sampler->sampler);
    }
}

void WaterRenderer::updateDescriptorSetResources(PerspectiveCamera* perspectiveCamera, Light* light) {
    WaterRendererVertexUniformBufferObject vertexUbo{};
    vertexUbo.viewMatrix = perspectiveCamera->createViewMatrix();
    vertexUbo.projectionMatrix = perspectiveCamera->createProjectionMatrix();
    vertexUbo.lightPosition = light->position;
    void* vubData;
    vkMapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory, 0, sizeof(vertexUbo), 0, &vubData);
    memcpy(vubData, &vertexUbo, sizeof(vertexUbo));
    vkUnmapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory);

    WaterRendererFragmentUniformBufferObject fragmentUbo{};
    moveFactor += waveSpeed * renderer->deltaTime;
    moveFactor = fmod(moveFactor, 1.0f);
    fragmentUbo.moveFactor = moveFactor;
    fragmentUbo.lightColor = light->color;
    fragmentUbo.nearPlane = perspectiveCamera->nearPlane;
    fragmentUbo.farPlane = perspectiveCamera->farPlane;
    fragmentUbo.fogColor = FOG_COLOR;
    void* fubData;
    vkMapMemory(renderer->device->device, fragmentUniformBuffers[renderer->currentFrame]->bufferMemory, 0, sizeof(fragmentUbo), 0, &fubData);
    memcpy(fubData, &fragmentUbo, sizeof(fragmentUbo));
    vkUnmapMemory(renderer->device->device, fragmentUniformBuffers[renderer->currentFrame]->bufferMemory);
}
