#include "entity.hpp"
#include "../rendering/texture.hpp"

Entity::Entity(Mesh* mesh, Texture* texture, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Renderer* renderer) {
    this->mesh = mesh;
    this->texture = texture;
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;

    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0] = {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2 * MAX_FRAMES_IN_FLIGHT};
    poolSizes[1] = {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_FRAMES_IN_FLIGHT};
    descriptorPool = new DescriptorPool(renderer->device->device, static_cast<uint32_t>(poolSizes.size()), poolSizes.data(), MAX_FRAMES_IN_FLIGHT);

    descriptorSets = new DescriptorSets(renderer->device->device, descriptorPool->descriptorPool, descriptorSetLayout->descriptorSetLayout, MAX_FRAMES_IN_FLIGHT);

    EntityFragmentUniformBufferObject ubo{};
    ubo.reflectivity = texture->reflectivity;
    ubo.shineDamper = texture->shineDamper;
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vertexUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(EntityVertexUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 0, 0, 1, vertexUniformBuffers[i]->buffer, sizeof(EntityVertexUniformBufferObject));

        fragmentUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(EntityFragmentUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 1, 0, 1, fragmentUniformBuffers[i]->buffer, sizeof(EntityFragmentUniformBufferObject));

        descriptorSets->updateImageInfo(i, 2, 0, 1, texture->image->imageView, texture->sampler->sampler);

        void* data;
        vkMapMemory(renderer->device->device, fragmentUniformBuffers[i]->bufferMemory, 0, sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(renderer->device->device, fragmentUniformBuffers[i]->bufferMemory);
    }
}

Entity::~Entity() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        delete vertexUniformBuffers[i];
        delete fragmentUniformBuffers[i];
    }
    vertexUniformBuffers.clear();
    fragmentUniformBuffers.clear();
    delete descriptorPool;
    delete mesh;
    delete texture;
}

void Entity::updateResources(uint32_t currentFrame) {
    glm::mat4 matrix(1.0f);
    matrix = glm::translate(matrix, position);
    matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    matrix = glm::scale(matrix, scale);

    EntityVertexUniformBufferObject ubo{};
    ubo.modelMatrix = matrix;

    void* data;
    vkMapMemory(device->device, vertexUniformBuffers[currentFrame]->bufferMemory, 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device->device, vertexUniformBuffers[currentFrame]->bufferMemory);
}

void Entity::CreateDescriptorSetLayout() {
    std::array<VkDescriptorSetLayoutBinding, 3> layoutBindings{};
    layoutBindings[0] = {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr};
    layoutBindings[1] = {1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
    layoutBindings[2] = {2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
    descriptorSetLayout = DescriptorSetLayout(renderer->device->device, static_cast<uint32_t>(layoutBindings.size()), layoutBindings.data());
}
