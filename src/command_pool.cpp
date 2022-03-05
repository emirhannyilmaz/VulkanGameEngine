#include "command_pool.hpp"

CommandPool::CommandPool(VkDevice device, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags) {
    this->device = device;

    VkCommandPoolCreateInfo commandPoolCreateInfo{};
    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;
    commandPoolCreateInfo.flags = flags;

    if (vkCreateCommandPool(device, &commandPoolCreateInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create command pool!");
    }
}

CommandPool::~CommandPool() {
    vkDestroyCommandPool(device, commandPool, nullptr);
}
