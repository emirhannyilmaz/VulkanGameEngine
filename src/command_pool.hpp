#ifndef command_pool_hpp
#define command_pool_hpp

#define VK_ENABLE_BETA_EXTENSIONS
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class CommandPool {
public:
    VkCommandPool commandPool;
    CommandPool(VkDevice device, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags);
    ~CommandPool();
private:
    VkDevice device;
};

#endif
