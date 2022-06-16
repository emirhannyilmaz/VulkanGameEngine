#ifndef sampler_hpp
#define sampler_hpp

#include <vulkan/vulkan.h>
#include <stdexcept>

class Sampler {
public:
    VkSampler sampler;
    Sampler(VkPhysicalDevice& physicalDevice, VkDevice& device, float maxLod);
    ~Sampler();
private:
    VkDevice device;
};

#endif
