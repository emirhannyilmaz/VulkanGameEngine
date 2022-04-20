#include "depth_resources.hpp"

DepthResources::DepthResources(VkPhysicalDevice& physicalDevice, VkDevice& device, VkExtent2D& swapchainExtent, VkSampleCountFlagBits msaaSamples, VkCommandPool& commandPool, VkQueue& graphicsQueue) {
    VkFormat format = findDepthFormat(physicalDevice);

    image = new Image(physicalDevice, device, swapchainExtent.width, swapchainExtent.height, 1, 1, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, msaaSamples, 0, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_DEPTH_BIT);
    Image::TransitionImageLayout(device, commandPool, graphicsQueue, image->image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1, 1, format);
}

DepthResources::~DepthResources() {
    delete image;
}

VkFormat DepthResources::findDepthFormat(VkPhysicalDevice& physicalDevice) {
    return findSupportedFormat(physicalDevice, {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

VkFormat DepthResources::findSupportedFormat(VkPhysicalDevice& physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
    for (VkFormat format : candidates) {
        VkFormatProperties properties;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &properties);

        if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features) {
            return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    throw std::runtime_error("Failed to find supported format!");
}
