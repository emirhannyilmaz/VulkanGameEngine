#ifndef image_hpp
#define image_hpp

#include <vulkan/vulkan.h>
#include "command_buffers.hpp"

class Image {
public:
    VkImage image;
    VkDeviceMemory imageMemory;
    VkImageView imageView;
    Image(VkPhysicalDevice& physicalDevice, VkDevice& device, uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageAspectFlags aspectFlags);
    ~Image();
    static void TransitionImageLayout(VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue, VkImage& image, uint32_t mipLevels, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    static void CopyBufferToImage(VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue, VkBuffer& buffer, VkImage& image, uint32_t width, uint32_t height);
    static void GenerateMipmaps(VkPhysicalDevice& physicalDevice, VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue, VkImage& image, VkFormat format, int32_t width, int32_t height, uint32_t mipLevels);
private:
    VkDevice device;
    uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
};

#endif
