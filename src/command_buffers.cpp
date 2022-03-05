#include "command_buffers.hpp"

CommandBuffers::CommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t count) {
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

void CommandBuffers::BeginRecordingCommandBuffer(uint32_t index) {
    VkCommandBufferBeginInfo commandBufferBeginInfo{};
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffers[index], &commandBufferBeginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }
}

void CommandBuffers::EndRecordingCommandBuffer(uint32_t index) {
    if (vkEndCommandBuffer(commandBuffers[index]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer!");
    }
}
