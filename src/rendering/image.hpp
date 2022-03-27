#ifndef image_hpp
#define image_hpp

#include <vulkan/vulkan.h>

class Image {
public:
    VkImage image;
    VkDeviceMemory imageMemory;
    Image(VkPhysicalDevice& physicalDevice, VkDevice device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
    ~Image();
private:
    VkDevice device;
    uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
};

#endif
