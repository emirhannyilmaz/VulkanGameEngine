#ifndef fence_hpp
#define fence_hpp

#include <vulkan/vulkan.h>
#include <stdexcept>

class Fence {
public:
    VkFence fence;
    Fence(VkDevice& device, VkFenceCreateFlags flags);
    ~Fence();
private:
    VkDevice device;
};

#endif
