#include "descriptor_set_layout.hpp"

DescriptorSetLayout::DescriptorSetLayout(VkDevice& device, uint32_t bindingCount, VkDescriptorSetLayoutBinding* bindings) {
    this->device = device;

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
    descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutCreateInfo.bindingCount = bindingCount;
    descriptorSetLayoutCreateInfo.pBindings = bindings;

    if (vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCreateInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create descriptor set layout!");
    }
}

DescriptorSetLayout::~DescriptorSetLayout() {
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
}
