#include "skybox.hpp"

DescriptorSetLayout* Skybox::descriptorSetLayout = nullptr;

Skybox::Skybox(Texture* texture, float size, Renderer* renderer) {
    this->texture = texture;
    this->size = size;
    this->renderer = renderer;

    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0] = {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_FRAMES_IN_FLIGHT};
    poolSizes[1] = {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_FRAMES_IN_FLIGHT};
    descriptorPool = new DescriptorPool(renderer->device->device, static_cast<uint32_t>(poolSizes.size()), poolSizes.data(), MAX_FRAMES_IN_FLIGHT);

    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout->descriptorSetLayout);
    descriptorSets = new DescriptorSets(renderer->device->device, descriptorPool->descriptorPool, layouts.data(), MAX_FRAMES_IN_FLIGHT);

    SkyboxVertexUniformBufferObject vertexUbo;
    vertexUbo.size = size;

    vertexUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vertexUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(SkyboxVertexUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 0, 0, 1, vertexUniformBuffers[i]->buffer, sizeof(SkyboxVertexUniformBufferObject));

        descriptorSets->updateImageInfo(i, 1, 0, 1, texture->image->imageView, texture->sampler->sampler);

        void* data;
        vkMapMemory(renderer->device->device, vertexUniformBuffers[i]->bufferMemory, 0, sizeof(vertexUbo), 0, &data);
        memcpy(data, &vertexUbo, sizeof(vertexUbo));
        vkUnmapMemory(renderer->device->device, vertexUniformBuffers[i]->bufferMemory);
    }
}

Skybox::~Skybox() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        delete vertexUniformBuffers[i];
    }
    vertexUniformBuffers.clear();
    delete descriptorPool;
    delete texture;
}

void Skybox::CreateDesriptorSetLayout(VkDevice& device) {
    std::array<VkDescriptorSetLayoutBinding, 2> layoutBindings{};
    layoutBindings[0] = {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr};
    layoutBindings[1] = {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
    descriptorSetLayout = new DescriptorSetLayout(device, static_cast<uint32_t>(layoutBindings.size()), layoutBindings.data());
}

void Skybox::DeleteDesriptorSetLayout() {
    delete descriptorSetLayout;
}
