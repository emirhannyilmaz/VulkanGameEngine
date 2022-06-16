#include "color_resources.hpp"

ColorResources::ColorResources(VkPhysicalDevice& physicalDevice, VkDevice& device, VkExtent2D& extent, VkSampleCountFlagBits msaaSamples, VkFormat format, VkImageUsageFlags usage) {
    image = new Image(physicalDevice, device, extent.width, extent.height, 1, 1, format, VK_IMAGE_TILING_OPTIMAL, usage, msaaSamples, 0, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
}

ColorResources::~ColorResources() {
    delete image;
}
