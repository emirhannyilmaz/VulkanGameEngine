#ifndef command_buffers_hpp
#define command_buffers_hpp

#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>

class CommandBuffers {
public:
    std::vector<VkCommandBuffer> commandBuffers;
    CommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t count);
    ~CommandBuffers();
private:
    VkDevice device;
    VkCommandPool commandPool;
};

#endif
