#ifndef command_pool_hpp
#define command_pool_hpp

#include <vulkan/vulkan.h>
#include <stdexcept>

class CommandPool {
public:
    VkCommandPool commandPool;
    CommandPool(VkDevice& device, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags);
    ~CommandPool();
private:
    VkDevice device;
};

#endif
