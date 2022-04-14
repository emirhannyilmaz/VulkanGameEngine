#include "entity.hpp"
#include "../rendering/texture.hpp"

Entity::Entity(Mesh* mesh, Texture* texture, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
    this->mesh = mesh;
    this->texture = texture;
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;

    const VkDescriptorPoolSize[] poolSizes = {
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2 * MAX_FRAMES_IN_FLIGHT},
        {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_FRAMES_IN_FLIGHT}
    };
    this->descriptorPool = new DescriptorPool(renderer->device->device, 2, poolSizes, MAX_FRAMES_IN_FLIGHT);

    const VkDescriptorSetLayoutBinding[] layoutBindings = {
        {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr},
        {1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr},
        {2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr},
    };
    this->descriptorSetLayout = new DescriptorSetLayout(renderer->device->device, 3, layoutBindings);

    this->descriptorSets = new DescriptorSets(renderer->device->device, descriptorPool->descriptorPool, descriptorSetLayout->descriptorSetLayout, MAX_FRAMES_IN_FLIGHT);

    VkDeviceSize uniformBufferSize = sizeof(UniformBufferObject);
    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        uniformBuffers[i] = new Buffer(device->physicalDevice, device->device, sizeof(EntityVertexUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    }
}

Entity::~Entity() {
    delete mesh;
    delete texture;
}

glm::mat4 Entity::createModelMatrix() {
    glm::mat4 matrix(1.0f);
    matrix = glm::translate(matrix, position);
    matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    matrix = glm::scale(matrix, scale);

    return matrix;
}
