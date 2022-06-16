#ifndef image_hpp
#define image_hpp

#include <vulkan/vulkan.h>
#include "command_buffers.hpp"

class Image {
public:
    VkImage image;
    VkDeviceMemory imageMemory;
    VkImageView imageView;
    Image(VkPhysicalDevice& physicalDevice, VkDevice& device, uint32_t width, uint32_t height, uint32_t mipLevels, uint32_t arrayLayers, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkSampleCountFlagBits numSamples, VkImageCreateFlags createFlags, VkMemoryPropertyFlags properties, VkImageViewType viewType, VkImageAspectFlags aspectFlags);
    ~Image();
    static void TransitionImageLayout(VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue, VkImage& image, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels, uint32_t arrayLayers, VkFormat format);
    static void CopyBufferToImage(VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue, uint32_t width, uint32_t height, uint32_t arrayLayers, VkBuffer& buffer, VkImage& image);
    static void GenerateMipmaps(VkPhysicalDevice& physicalDevice, VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue, VkImage& image, VkFormat format, int32_t width, int32_t height, uint32_t mipLevels);
private:
    VkDevice device;
    uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
};

#endif
