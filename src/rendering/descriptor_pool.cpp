#include "descriptor_pool.hpp"

DescriptorPool::DescriptorPool(VkDevice& device, uint32_t poolSizeCount, VkDescriptorPoolSize* poolSizes, uint32_t maxSets) {
    this->device = device;

    VkDescriptorPoolCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.poolSizeCount = poolSizeCount;
    createInfo.pPoolSizes = poolSizes;
    createInfo.maxSets = maxSets;

    if (vkCreateDescriptorPool(device, &createInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create descriptor pool!");
    }
}

DescriptorPool::~DescriptorPool() {
    vkDestroyDescriptorPool(device, descriptorPool, nullptr);
}
