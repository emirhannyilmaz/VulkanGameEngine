#ifndef descriptor_set_layout_hpp
#define descriptor_set_layout_hpp

#include <vulkan/vulkan.h>
#include <stdexcept>

class DescriptorSetLayout {
public:
    VkDescriptorSetLayout descriptorSetLayout;
    DescriptorSetLayout(const VkDevice& device, uint32_t bindingCount, const VkDescriptorSetLayoutBinding* bindings);
    ~DescriptorSetLayout();
private:
    VkDevice device;
};

#endif
