#ifndef descriptor_sets_hpp
#define descriptor_sets_hpp

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>

class DescriptorSets {
public:
    std::vector<VkDescriptorSet> descriptorSets;
    DescriptorSets(VkDevice& device, VkDescriptorPool& pool, VkDescriptorSetLayout* layouts, uint32_t descriptorSetCount);
    ~DescriptorSets();
    void updateBufferInfo(size_t index, uint32_t binding, uint32_t element, uint32_t count, VkBuffer& buffer, VkDeviceSize range);
    void updateImageInfo(size_t index, uint32_t binding, uint32_t element, uint32_t count, VkImageView& imageView, VkSampler& sampler);
private:
    VkDevice device;
    VkDescriptorPool pool;
};

#endif
