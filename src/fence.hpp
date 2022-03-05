#ifndef fence_hpp
#define fence_hpp

#define VK_ENABLE_BETA_EXTENSIONS
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Fence {
public:
    VkFence fence;
    Fence(VkDevice device, VkFenceCreateFlags flags);
    ~Fence();
private:
    VkDevice device;
};

#endif
