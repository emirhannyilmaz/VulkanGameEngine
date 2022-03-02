#ifndef swapchain_hpp
#define swapchain_hpp

#define VK_ENABLE_BETA_EXTENSIONS
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct SwapchainSupportDetails;
struct QueueFamilyIndices;

class Swapchain {
public:
    VkSwapchainKHR swapchain;
    VkFormat swapchainImageFormat;
    VkExtent2D swapchainExtent;
    std::vector<VkImageView> swapchainImageViews;
    Swapchain(VkDevice device, VkSurfaceKHR surface, SwapchainSupportDetails swapchainSupportDetails, GLFWwindow* window, QueueFamilyIndices indices);
    ~Swapchain();
private:
    VkDevice device;
    std::vector<VkImage> swapchainImages;
    VkSurfaceFormatKHR chooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapchainPresentMode(const std::vector<VkPresentModeKHR>& availableModes);
    VkExtent2D chooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
};

#endif
