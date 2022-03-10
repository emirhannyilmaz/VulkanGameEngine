#include "command_buffers.hpp"

CommandBuffers::CommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t count) {
    this->device = device;
    this->commandPool = commandPool;

    commandBuffers.resize(count);

    VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.commandPool = commandPool;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandBufferCount = count;

    if (vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffers!");
    }
}

CommandBuffers::~CommandBuffers() {
    vkFreeCommandBuffers(device, commandPool, commandBuffers.size(), commandBuffers.data());
}
