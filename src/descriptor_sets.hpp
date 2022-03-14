#ifndef descriptor_sets_hpp
#define descriptor_sets_hpp

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>

class DescriptorSets {
public:
    std::vector<VkDescriptorSet> descriptorSets;
    DescriptorSets(VkDevice& device, VkDescriptorPool& pool, VkDescriptorSetLayout& layout, std::vector<VkBuffer&> uniformBuffers);
    ~DescriptorSets();
private:
    VkDevice device;
    VkDescriptorPool pool;
};

#endif
