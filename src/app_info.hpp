#ifndef app_info_hpp
#define app_info_hpp

#define VK_ENABLE_BETA_EXTENSIONS
#include <vulkan/vulkan.h>
#include <string>
#include <vector>

const std::string TITLE = "Vulkan Game Engine";

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation",
};

const std::vector<const char*> deviceExtensions = {
    "VK_KHR_swapchain",
    "VK_KHR_portability_subset"
};

const int MAX_FRAMES_IN_FLIGHT = 2;

#endif
