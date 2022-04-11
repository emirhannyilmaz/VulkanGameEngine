#include "color_resources.hpp"

ColorResources::ColorResources(VkPhysicalDevice& physicalDevice, VkDevice& device, VkExtent2D& swapchainExtent, VkSampleCountFlagBits msaaSamples, VkFormat swapchainImageFormat) {
    image = new Image(physicalDevice, device, swapchainExtent.width, swapchainExtent.height, 1, msaaSamples, swapchainImageFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT);
}

ColorResources::~ColorResources() {
    delete image;
}
