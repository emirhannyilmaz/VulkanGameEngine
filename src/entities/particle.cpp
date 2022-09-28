#include "particle.hpp"

DescriptorSetLayout* Particle::descriptorSetLayout = nullptr;

Particle::Particle(TextureAtlas* textureAtlas, glm::vec3 position, float rotation, glm::vec3 scale, glm::vec3 velocity, float gravityMultiplier, float lifeLength, Renderer* renderer) {
    this->textureAtlas = textureAtlas;
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
    this->velocity = velocity;
    this->gravityMultiplier = gravityMultiplier;
    this->lifeLength = lifeLength;
    this->renderer = renderer;

    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0] = {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2 * MAX_FRAMES_IN_FLIGHT};
    poolSizes[1] = {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_FRAMES_IN_FLIGHT};
    descriptorPool = new DescriptorPool(renderer->device->device, static_cast<uint32_t>(poolSizes.size()), poolSizes.data(), MAX_FRAMES_IN_FLIGHT);

    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout->descriptorSetLayout);
    descriptorSets = new DescriptorSets(renderer->device->device, descriptorPool->descriptorPool, layouts.data(), MAX_FRAMES_IN_FLIGHT);

    vertexUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    fragmentUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vertexUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(ParticleVertexUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 0, 0, 1, vertexUniformBuffers[i]->buffer, sizeof(ParticleVertexUniformBufferObject));

        fragmentUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(ParticleFragmentUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 1, 0, 1, fragmentUniformBuffers[i]->buffer, sizeof(ParticleFragmentUniformBufferObject));

        descriptorSets->updateImageInfo(i, 2, 0, 1, textureAtlas->image->imageView, textureAtlas->sampler->sampler);
    }
}

Particle::~Particle() {
    delete textureAtlas;
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        delete vertexUniformBuffers[i];
        delete fragmentUniformBuffers[i];
    }
    vertexUniformBuffers.clear();
    fragmentUniformBuffers.clear();
    delete descriptorPool;
}

bool Particle::update(PerspectiveCamera* perspectiveCamera, float deltaTime, float realDeltaTime) {
    distanceFromCamera = glm::length(perspectiveCamera->position - position);
    velocity.y += GRAVITY * gravityMultiplier * deltaTime;
    position += velocity * deltaTime;
    updateTextureAtlasInfo();
    elapsedTime += realDeltaTime;

    return elapsedTime < lifeLength;
}

void Particle::updateTextureAtlasInfo() {
    float lifeFactor = elapsedTime / lifeLength;
    float atlasProgression = lifeFactor * textureAtlas->imageCount;
    int index1 = (int) std::floor(atlasProgression);
    int index2 = index1 < textureAtlas->imageCount - 1 ? index1 + 1 : index1;
    blendFactor = fmod(atlasProgression, 1);
    textureOffset1 = textureAtlas->getOffsetAtIndex(index1);
    textureOffset2 = textureAtlas->getOffsetAtIndex(index2);
}

void Particle::updateDescriptorSetResources() {
    glm::mat4 matrix(1.0f);
    matrix = glm::translate(matrix, position);
    matrix = glm::rotate(matrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    matrix = glm::scale(matrix, scale);

    ParticleVertexUniformBufferObject vertexUbo{};
    vertexUbo.modelMatrix = matrix;
    vertexUbo.textureOffset1 = textureOffset1;
    vertexUbo.textureOffset2 = textureOffset2;
    vertexUbo.rowCount = textureAtlas->rowCount;

    void* data;
    vkMapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory, 0, sizeof(vertexUbo), 0, &data);
    memcpy(data, &vertexUbo, sizeof(vertexUbo));
    vkUnmapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory);

    ParticleFragmentUniformBufferObject fragmentUbo{};
    fragmentUbo.blendFactor = blendFactor;

    void* data2;
    vkMapMemory(renderer->device->device, fragmentUniformBuffers[renderer->currentFrame]->bufferMemory, 0, sizeof(fragmentUbo), 0, &data2);
    memcpy(data2, &fragmentUbo, sizeof(fragmentUbo));
    vkUnmapMemory(renderer->device->device, fragmentUniformBuffers[renderer->currentFrame]->bufferMemory);
}

void Particle::updatePushConstants(PerspectiveCamera* perspectiveCamera) {
    glm::mat4 modelMatrix(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    glm::mat4 viewMatrix = perspectiveCamera->createViewMatrix();
    modelMatrix[0][0] = viewMatrix[0][0];
    modelMatrix[0][1] = viewMatrix[1][0];
    modelMatrix[0][2] = viewMatrix[2][0];
    modelMatrix[1][0] = viewMatrix[0][1];
    modelMatrix[1][1] = viewMatrix[1][1];
    modelMatrix[1][2] = viewMatrix[2][1];
    modelMatrix[2][0] = viewMatrix[0][2];
    modelMatrix[2][1] = viewMatrix[1][2];
    modelMatrix[2][2] = viewMatrix[2][2];
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, scale);

    vertexPushConstants.modelViewMatrix = viewMatrix * modelMatrix;
}

void Particle::CreateDesriptorSetLayout(VkDevice& device) {
    std::array<VkDescriptorSetLayoutBinding, 3> layoutBindings{};
    layoutBindings[0] = {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr};
    layoutBindings[1] = {1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
    layoutBindings[2] = {2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
    descriptorSetLayout = new DescriptorSetLayout(device, static_cast<uint32_t>(layoutBindings.size()), layoutBindings.data());
}

void Particle::DeleteDesriptorSetLayout() {
    delete descriptorSetLayout;
}
