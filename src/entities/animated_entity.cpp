#include "animated_entity.hpp"
#include "../rendering/renderer/texture.hpp"
#include "../rendering/renderer/renderer.hpp"

DescriptorSetLayout* AnimatedEntity::descriptorSetLayout = nullptr;

AnimatedEntity::AnimatedEntity(AnimatedEntityMesh* mesh, Texture* texture, Joint* rootJoint, int jointCount, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Renderer* renderer) {
    this->mesh = mesh;
    this->texture = texture;
    this->rootJoint = rootJoint;
    this->jointCount = jointCount;
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
    this->renderer = renderer;
    
    rootJoint->calculateInverseBindTransform(glm::mat4(1.0f));

    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0] = {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2 * MAX_FRAMES_IN_FLIGHT};
    poolSizes[1] = {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_FRAMES_IN_FLIGHT};
    descriptorPool = new DescriptorPool(renderer->device->device, static_cast<uint32_t>(poolSizes.size()), poolSizes.data(), MAX_FRAMES_IN_FLIGHT);

    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout->descriptorSetLayout);
    descriptorSets = new DescriptorSets(renderer->device->device, descriptorPool->descriptorPool, layouts.data(), MAX_FRAMES_IN_FLIGHT);

    AnimatedEntityFragmentUniformBufferObject ubo{};
    ubo.reflectivity = texture->reflectivity;
    ubo.shineDamper = texture->shineDamper;

    vertexUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    fragmentUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vertexUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(AnimatedEntityVertexUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 0, 0, 1, vertexUniformBuffers[i]->buffer, sizeof(AnimatedEntityVertexUniformBufferObject));

        fragmentUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(AnimatedEntityFragmentUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 1, 0, 1, fragmentUniformBuffers[i]->buffer, sizeof(AnimatedEntityFragmentUniformBufferObject));

        descriptorSets->updateImageInfo(i, 2, 0, 1, texture->image->imageView, texture->sampler->sampler);

        void* data;
        vkMapMemory(renderer->device->device, fragmentUniformBuffers[i]->bufferMemory, 0, sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(renderer->device->device, fragmentUniformBuffers[i]->bufferMemory);
    }
}

AnimatedEntity::~AnimatedEntity() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        delete vertexUniformBuffers[i];
        delete fragmentUniformBuffers[i];
    }
    vertexUniformBuffers.clear();
    fragmentUniformBuffers.clear();
    delete descriptorPool;
    delete mesh;
    delete texture;
    delete rootJoint;
}

std::vector<glm::mat4> AnimatedEntity::getJointTransforms() {
    std::vector<glm::mat4> jointTransforms(jointCount);
    addJointTransformToVector(rootJoint, jointTransforms);
    return jointTransforms;
}

void AnimatedEntity::addJointTransformToVector(Joint* parentJoint, std::vector<glm::mat4>& jointTransforms) {
    jointTransforms[parentJoint->index] = parentJoint->animatedTransform;
    for (Joint* childJoint : parentJoint->children) {
        addJointTransformToVector(childJoint, jointTransforms);
    }
}

void AnimatedEntity::updateDescriptorSetResources() {
    glm::mat4 matrix(1.0f);
    matrix = glm::translate(matrix, position);
    matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    matrix = glm::scale(matrix, scale);

    AnimatedEntityVertexUniformBufferObject vertexUbo{};
    vertexUbo.modelMatrix = matrix;
    std::vector<glm::mat4> jointTransforms = getJointTransforms();
    memcpy(vertexUbo.jointTransforms, jointTransforms.data(), sizeof(glm::mat4) * jointTransforms.size());

    void* data;
    vkMapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory, 0, sizeof(vertexUbo), 0, &data);
    memcpy(data, &vertexUbo, sizeof(vertexUbo));
    vkUnmapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory);
}

void AnimatedEntity::CreateDesriptorSetLayout(VkDevice& device) {
    std::array<VkDescriptorSetLayoutBinding, 3> layoutBindings{};
    layoutBindings[0] = {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr};
    layoutBindings[1] = {1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
    layoutBindings[2] = {2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
    descriptorSetLayout = new DescriptorSetLayout(device, static_cast<uint32_t>(layoutBindings.size()), layoutBindings.data());
}

void AnimatedEntity::DeleteDesriptorSetLayout() {
    delete descriptorSetLayout;
}
