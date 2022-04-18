#ifndef swapchain_hpp
#define swapchain_hpp

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>
#include "device.hpp"

class Swapchain {
public:
    VkSwapchainKHR swapchain;
    VkFormat swapchainImageFormat;
    VkExtent2D swapchainExtent;
    std::vector<VkImageView> swapchainImageViews;
    Swapchain(VkDevice& device, VkSurfaceKHR& surface, SwapchainSupportDetails& swapchainSupportDetails, GLFWwindow* window, QueueFamilyIndices& indices);
    ~Swapchain();
private:
    VkDevice device;
    std::vector<VkImage> swapchainImages;
    VkSurfaceFormatKHR chooseSwapchainSurfaceFormat(std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapchainPresentMode(std::vector<VkPresentModeKHR>& availableModes);
    VkExtent2D chooseSwapchainExtent(VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
};

#endif
