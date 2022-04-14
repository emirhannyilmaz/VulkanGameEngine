#include "descriptor_sets.hpp"

DescriptorSets::DescriptorSets(const VkDevice& device, const VkDescriptorPool& pool, const VkDescriptorSetLayout* layouts, uint32_t descriptorSetCount) {
    this->device = device;
    this->pool = pool;

    VkDescriptorSetAllocateInfo allocateInfo{};
    allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocateInfo.descriptorPool = pool;
    allocateInfo.pSetLayouts = layouts;
    allocateInfo.descriptorSetCount = descriptorSetCount;

    descriptorSets.resize(descriptorSetCount);
    if (vkAllocateDescriptorSets(device, &allocateInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate descriptor sets!");
    }
}

DescriptorSets::~DescriptorSets() {
    vkFreeDescriptorSets(device, pool, descriptorSets.size(), descriptorSets.data());
}

void DescriptorSets::updateBufferInfo(int index, uint32_t binding, uint32_t element, uint32_t count, const VkBuffer& buffer, VkDeviceSize range) {
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = buffer;
    bufferInfo.offset = 0;
    bufferInfo.range = range;

    VkWriteDescriptorSet writeDescriptorSet{};
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.dstSet = descriptorSets[index];
    writeDescriptorSet.dstBinding = binding;
    writeDescriptorSet.dstArrayElement = element;
    writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writeDescriptorSet.descriptorCount = count;
    writeDescriptorSet.pBufferInfo = &bufferInfo;

    vkUpdateDescriptorSets(device, 1, &writeDescriptorSet, 0, nullptr);
}

void DescriptorSets::updateImageInfo(int index, uint32_t binding, uint32_t element, uint32_t count, const VkImageView& imageView, const VkSampler& sampler) {
    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = imageView;
    imageInfo.sampler = sampler;

    VkWriteDescriptorSet writeDescriptorSet{};
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.dstSet = descriptorSets[index];
    writeDescriptorSet.dstBinding = binding;
    writeDescriptorSet.dstArrayElement = element;
    writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDescriptorSet.descriptorCount = count;
    writeDescriptorSet.pImageInfo = &imageInfo;

    vkUpdateDescriptorSets(device, 1, &writeDescriptorSet, 0, nullptr);
}
