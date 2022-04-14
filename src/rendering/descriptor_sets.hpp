#ifndef descriptor_sets_hpp
#define descriptor_sets_hpp

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>

class DescriptorSets {
public:
    std::vector<VkDescriptorSet> descriptorSets;
    DescriptorSets(const VkDevice& device, const VkDescriptorPool& pool, const VkDescriptorSetLayout* layouts, uint32_t descriptorSetCount);
    ~DescriptorSets();
    void updateBufferInfo(int index, uint32_t binding, uint32_t element, uint32_t count, const VkBuffer& buffer, VkDeviceSize range);
    void updateImageInfo(int index, uint32_t binding, uint32_t element, uint32_t count, const VkImageView& imageView, const VkSampler& sampler);
private:
    VkDevice device;
    VkDescriptorPool pool;
};

#endif
