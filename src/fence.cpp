#include "fence.hpp"

Fence::Fence(VkDevice device, VkFenceCreateFlags flags) {
    this->device = device;

    VkFenceCreateInfo fenceCreateInfo{};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = flags;

    if (vkCreateFence(device, &fenceCreateInfo, nullptr, &fence) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create fence!");
    }
}

Fence::~Fence() {
    vkDestroyFence(device, fence, nullptr);
}
