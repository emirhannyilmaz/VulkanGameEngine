#ifndef buffer_hpp
#define buffer_hpp

#include <vulkan/vulkan.h>
#include <vector>
#include "vertex.hpp"
#include <stdexcept>
#include "command_buffers.hpp"

class Buffer {
public:
    VkBuffer buffer;
    VkDeviceMemory bufferMemory;
    Buffer(VkPhysicalDevice& physicalDevice, VkDevice& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
    ~Buffer();
    static void Copy(VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue, VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize size);
private:
    VkDevice device;
    uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
};

#endif
