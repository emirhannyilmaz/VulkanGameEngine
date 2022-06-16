#ifndef shadow_map_resources_hpp
#define shadow_map_resources_hpp

#include "depth_resources.hpp"
#include "sampler.hpp"
#include "render_pass.hpp"
#include "framebuffer.hpp"
#include <array>

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
