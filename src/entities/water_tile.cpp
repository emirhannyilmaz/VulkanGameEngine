#include "water_tile.hpp"

DescriptorSetLayout* WaterTile::descriptorSetLayout = nullptr;

WaterTile::WaterTile(glm::vec3 position, glm::vec2 scale, Renderer* renderer) {
/*
    this->position = position;
    this->scale = scale;

    std::array<VkDescriptorPoolSize, 1> poolSizes{};
    poolSizes[0] = {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_FRAMES_IN_FLIGHT};
    descriptorPool = new DescriptorPool(renderer->device->device, static_cast<uint32_t>(poolSizes.size()), poolSizes.data(), MAX_FRAMES_IN_FLIGHT);

    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout->descriptorSetLayout);
    descriptorSets = new DescriptorSets(renderer->device->device, descriptorPool->descriptorPool, layouts.data(), MAX_FRAMES_IN_FLIGHT);

    vertexUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        uniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(WaterTileVertexUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 0, 0, 1, uniformBuffers[i]->buffer, sizeof(WaterTileVertexUniformBufferObject));

        void* data;
        vkMapMemory(renderer->device->device, uniformBuffers[i]->bufferMemory, 0, sizeof(vertexUbo), 0, &data);
        memcpy(data, &vertexUbo, sizeof(vertexUbo));
        vkUnmapMemory(renderer->device->device, uniformBuffers[i]->bufferMemory);
    }
*/
}

WaterTile::~WaterTile() {
}

void WaterTile::updateDescriptorSetResources() {
/*
    EntityVertexUniformBufferObject ubo{};
    ubo.modelMatrix = matrix;

    void* data;
    vkMapMemory(renderer->device->device, vertexUniformBuffers[currentFrame]->bufferMemory, 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(renderer->device->device, vertexUniformBuffers[currentFrame]->bufferMemory);
*/
}

void WaterTile::CreateDesriptorSetLayout(VkDevice& device) {
    std::array<VkDescriptorSetLayoutBinding, 1> layoutBindings{};
    layoutBindings[0] = {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr};
    descriptorSetLayout = new DescriptorSetLayout(device, static_cast<uint32_t>(layoutBindings.size()), layoutBindings.data());
}

void WaterTile::DeleteDesriptorSetLayout() {
    delete descriptorSetLayout;
}
