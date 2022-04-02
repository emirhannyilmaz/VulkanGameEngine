#ifndef descriptor_pool_hpp
#define descriptor_pool_hpp

#include <vulkan/vulkan.h>
#include <stdexcept>
#include "renderer_info.hpp"
#include <array>

class DescriptorPool {
public:
    VkDescriptorPool descriptorPool;
    DescriptorPool(VkDevice& device);
    ~DescriptorPool();
private:
    VkDevice device;
};

#endif
