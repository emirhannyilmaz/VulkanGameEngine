#include "buffer.hpp"

Buffer::Buffer(VkPhysicalDevice& physicalDevice, VkDevice& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) {
    this->device = device;

    VkBufferCreateInfo bufferCreateInfo{};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size = size;
    bufferCreateInfo.usage = usage;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferCreateInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create buffer!");
    }

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memoryRequirements);

    VkMemoryAllocateInfo memoryAllocateInfo{};
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = findMemoryType(physicalDevice, memoryRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate buffer memory!");
    }

    vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

Buffer::~Buffer() {
    vkDestroyBuffer(device, buffer, nullptr);
    vkFreeMemory(device, bufferMemory, nullptr);
}

void Buffer::Copy(VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue, VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize size) {
    CommandBuffers commandBuffers(device, commandPool, 1);

    VkCommandBufferBeginInfo commandBufferBeginInfo{};
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if (vkBeginCommandBuffer(commandBuffers.commandBuffers[0], &commandBufferBeginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;

    vkCmdCopyBuffer(commandBuffers.commandBuffers[0], srcBuffer, dstBuffer, 1, &copyRegion);

    if (vkEndCommandBuffer(commandBuffers.commandBuffers[0]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer!");
    }

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers.commandBuffers[0];

    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);
}

uint32_t Buffer::findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties)) {
            return i;
        }
    }

    throw std::runtime_error("Failed to find suitable memory type!");
}
