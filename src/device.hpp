#ifndef device_hpp
#define device_hpp

#define VK_ENABLE_BETA_EXTENSIONS
#include <vulkan/vulkan.h>
#include <optional>
#include <set>
#include <stdexcept>
#include "app_info.hpp"

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapchainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class Device {
public:
    VkDevice device;
    QueueFamilyIndices indices;
    SwapchainSupportDetails swapchainSupportDetails;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    Device(VkInstance instance, VkSurfaceKHR surface);
    ~Device();
private:
    bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
};

#endif
