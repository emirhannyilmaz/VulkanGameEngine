#ifndef common_hpp
#define common_hpp

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>

const bool ENABLE_VALIDATION_LAYERS = true;

const std::vector<const char*> VALIDATION_LAYERS = {
    "VK_LAYER_KHRONOS_validation",
};

const std::vector<const char*> DEVICE_EXTENSIONS = {
    "VK_KHR_swapchain",
    "VK_KHR_portability_subset"
};

const int MAX_FRAMES_IN_FLIGHT = 2;
const uint32_t SHADOW_MAP_SIZE = 4096;
const float SHADOW_DISTANCE = 400.0f;
const glm::vec3 FOG_COLOR = glm::vec3(0.5444f, 0.62f, 0.69f);
const float GRAVITY = 50.0f;

#endif
