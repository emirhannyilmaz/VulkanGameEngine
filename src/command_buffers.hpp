#ifndef command_buffers_hpp
#define command_buffers_hpp

#define VK_ENABLE_BETA_EXTENSIONS
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class CommandBuffers {
public:
    std::vector<VkCommandBuffer> commandBuffers;
    CommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t count);
};

#endif
