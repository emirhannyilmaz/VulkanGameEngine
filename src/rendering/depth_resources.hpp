#ifndef depth_resources_hpp
#define depth_resources_hpp

#include "image.hpp"

class DepthResources {
public:
    Image* image;
    DepthResources(VkPhysicalDevice& physicalDevice, VkDevice& device, VkExtent2D& swapchainExtent, VkSampleCountFlagBits msaaSamples, VkCommandPool& commandPool, VkQueue& graphicsQueue);
    ~DepthResources();
    static VkFormat findDepthFormat(VkPhysicalDevice& physicalDevice);
private:
    static VkFormat findSupportedFormat(VkPhysicalDevice& physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
};

#endif
