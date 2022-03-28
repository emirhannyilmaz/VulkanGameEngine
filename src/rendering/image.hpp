#ifndef image_hpp
#define image_hpp

#include <vulkan/vulkan.h>
#include "command_buffers.hpp"

class Image {
public:
    VkImage image;
    VkDeviceMemory imageMemory;
    Image(VkPhysicalDevice& physicalDevice, VkDevice device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
    ~Image();
    static void TransitionImageLayout(VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue, VkImage& image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    static void CopyBufferToImage(VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue, VkBuffer& buffer, VkImage& image, uint32_t width, uint32_t height);
private:
    VkDevice device;
    uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
};

#endif
