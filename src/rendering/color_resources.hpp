#ifndef color_resources_hpp
#define color_resources_hpp

#include "image.hpp"

class ColorResources {
public:
    Image* image;
    ColorResources(VkPhysicalDevice& physicalDevice, VkDevice& device, VkExtent2D& extent, VkSampleCountFlagBits msaaSamples, VkFormat format, VkImageUsageFlags usage);
    ~ColorResources();
};

#endif
