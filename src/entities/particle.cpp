#include "particle.hpp"

DescriptorSetLayout* Particle::descriptorSetLayout = nullptr;

Particle::Particle(glm::vec3 position, float rotation, glm::vec3 scale, glm::vec3 velocity, float gravityMultiplier, float lifeLength, Renderer* renderer) {
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
    this->velocity = velocity;
    this->gravityMultiplier = gravityMultiplier;
    this->lifeLength = lifeLength;
    this->renderer = renderer;

    std::array<VkDescriptorPoolSize, 1> poolSizes{};
    poolSizes[0] = {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_FRAMES_IN_FLIGHT};
    descriptorPool = new DescriptorPool(renderer->device->device, static_cast<uint32_t>(poolSizes.size()), poolSizes.data(), MAX_FRAMES_IN_FLIGHT);

    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout->descriptorSetLayout);
    descriptorSets = new DescriptorSets(renderer->device->device, descriptorPool->descriptorPool, layouts.data(), MAX_FRAMES_IN_FLIGHT);

    vertexUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vertexUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(ParticleVertexUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 0, 0, 1, vertexUniformBuffers[i]->buffer, sizeof(ParticleVertexUniformBufferObject));
    }
}

Particle::~Particle() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        delete vertexUniformBuffers[i];
    }
    vertexUniformBuffers.clear();
    delete descriptorPool;
}

bool Particle::update(float deltaTime) {
    velocity.y += GRAVITY * gravityMultiplier * deltaTime;
    position += velocity * deltaTime;
    elapsedTime += deltaTime;
    return elapsedTime < lifeLength;
}

void Particle::updateDescriptorSetResources() {
    glm::mat4 matrix(1.0f);
    matrix = glm::translate(matrix, position);
    matrix = glm::rotate(matrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    matrix = glm::scale(matrix, scale);

    ParticleVertexUniformBufferObject vertexUbo{};
    vertexUbo.modelMatrix = matrix;

    void* data;
    vkMapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory, 0, sizeof(vertexUbo), 0, &data);
    memcpy(data, &vertexUbo, sizeof(vertexUbo));
    vkUnmapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory);
}

void Particle::CreateDesriptorSetLayout(VkDevice& device) {
    std::array<VkDescriptorSetLayoutBinding, 1> layoutBindings{};
    layoutBindings[0] = {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr};
    descriptorSetLayout = new DescriptorSetLayout(device, static_cast<uint32_t>(layoutBindings.size()), layoutBindings.data());
}

void Particle::DeleteDesriptorSetLayout() {
    delete descriptorSetLayout;
}
