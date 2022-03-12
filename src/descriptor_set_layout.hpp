#ifndef descriptor_set_layout_hpp
#define descriptor_set_layout_hpp

#include <vulkan/vulkan.h>
#include <stdexcept>

class DescriptorSetLayout {
public:
    VkDescriptorSetLayout descriptorSetLayout;
    DescriptorSetLayout(VkDevice& device);
    ~DescriptorSetLayout();
private:
    VkDevice device;
};

#endif
