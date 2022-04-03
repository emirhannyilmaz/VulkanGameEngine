#ifndef depth_resources_hpp
#define depth_resources_hpp

#include "image.hpp"

class DepthResources {
public:
    Image* image;
    DepthResources(VkPhysicalDevice& physicalDevice, VkDevice& device, VkExtent2D& swapchainExtent, VkCommandPool& commandPool, VkQueue& graphicsQueue);
    ~DepthResources();
private:
    VkFormat findSupportedFormat(VkPhysicalDevice& physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    VkFormat findDepthFormat(VkPhysicalDevice& physicalDevice);
    bool hasStencilComponent(VkFormat format);
};

#endif
