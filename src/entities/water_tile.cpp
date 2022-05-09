#include "water_tile.hpp"

DescriptorSetLayout* WaterTile::descriptorSetLayout = nullptr;

WaterTile::WaterTile(glm::vec3 position, glm::vec2 scale, float reflectivity, float shineDamper, Renderer* renderer) {
    this->position = position;
    this->scale = scale;
    this->reflectivity = reflectivity;
    this->shineDamper = shineDamper;
    this->renderer = renderer;

    std::array<VkDescriptorPoolSize, 1> poolSizes{};
    poolSizes[0] = {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2 * MAX_FRAMES_IN_FLIGHT};
    descriptorPool = new DescriptorPool(renderer->device->device, static_cast<uint32_t>(poolSizes.size()), poolSizes.data(), MAX_FRAMES_IN_FLIGHT);

    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout->descriptorSetLayout);
    descriptorSets = new DescriptorSets(renderer->device->device, descriptorPool->descriptorPool, layouts.data(), MAX_FRAMES_IN_FLIGHT);

    vertexUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    fragmentUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vertexUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(WaterTileVertexUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 0, 0, 1, vertexUniformBuffers[i]->buffer, sizeof(WaterTileVertexUniformBufferObject));

        fragmentUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(WaterTileFragmentUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 1, 0, 1, fragmentUniformBuffers[i]->buffer, sizeof(WaterTileFragmentUniformBufferObject));
    }
}

WaterTile::~WaterTile() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        delete fragmentUniformBuffers[i];
        delete vertexUniformBuffers[i];
    }
    fragmentUniformBuffers.clear();
    vertexUniformBuffers.clear();
    delete descriptorPool;
}

void WaterTile::updateDescriptorSetResources() {
    glm::mat4 matrix(1.0f);
    matrix = glm::translate(matrix, position);
    matrix = glm::scale(matrix, glm::vec3(scale.x, 1.0f, scale.y));
    WaterTileVertexUniformBufferObject vertexUbo{};
    vertexUbo.modelMatrix = matrix;
    void* vubData;
    vkMapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory, 0, sizeof(vertexUbo), 0, &vubData);
    memcpy(vubData, &vertexUbo, sizeof(vertexUbo));
    vkUnmapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory);

    WaterTileFragmentUniformBufferObject fragmentUbo{};
    fragmentUbo.reflectivity = reflectivity;
    fragmentUbo.shineDamper = shineDamper;
    void* fubData;
    vkMapMemory(renderer->device->device, fragmentUniformBuffers[renderer->currentFrame]->bufferMemory, 0, sizeof(fragmentUbo), 0, &fubData);
    memcpy(fubData, &fragmentUbo, sizeof(fragmentUbo));
    vkUnmapMemory(renderer->device->device, fragmentUniformBuffers[renderer->currentFrame]->bufferMemory);
}

void WaterTile::CreateDesriptorSetLayout(VkDevice& device) {
    std::array<VkDescriptorSetLayoutBinding, 2> layoutBindings{};
    layoutBindings[0] = {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr};
    layoutBindings[1] = {1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
    descriptorSetLayout = new DescriptorSetLayout(device, static_cast<uint32_t>(layoutBindings.size()), layoutBindings.data());
}

void WaterTile::DeleteDesriptorSetLayout() {
    delete descriptorSetLayout;
}
