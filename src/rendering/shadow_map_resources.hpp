#ifndef shadow_map_resources_hpp
#define shadow_map_resources_hpp

class ShadowMapResources {
public:
    DepthResources* depthResources;
    Sampler* sampler;
    RenderPass* renderPass;
    Framebuffer* framebuffer;
    ShadowMapResources(VkPhysicalDevice& physicalDevice, VkDevice& device, VkExtent2D& extent);
    ~ShadowMapResources();
};

#endif
