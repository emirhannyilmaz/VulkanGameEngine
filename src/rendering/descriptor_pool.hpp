#ifndef descriptor_pool_hpp
#define descriptor_pool_hpp

#include <vulkan/vulkan.h>
#include <stdexcept>

class DescriptorPool {
public:
    VkDescriptorPool descriptorPool;
    DescriptorPool(VkDevice& device, uint32_t poolSizeCount, VkDescriptorPoolSize* poolSizes, uint32_t maxSets);
    ~DescriptorPool();
private:
    VkDevice device;
};

#endif
