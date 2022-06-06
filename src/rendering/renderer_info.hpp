#ifndef renderer_info_hpp
#define renderer_info_hpp

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

const bool enableValidationLayers = false;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation",
};

const std::vector<const char*> deviceExtensions = {
    "VK_KHR_swapchain",
    "VK_KHR_portability_subset"
};

const int MAX_FRAMES_IN_FLIGHT = 2;
const uint32_t SHADOW_MAP_SIZE = 2048;

#endif
