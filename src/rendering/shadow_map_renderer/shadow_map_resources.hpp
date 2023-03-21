#ifndef shadow_map_resources_hpp
#define shadow_map_resources_hpp

#include "../renderer/depth_resources.hpp"
#include "../renderer/sampler.hpp"
#include "shadow_map_renderer_render_pass.hpp"
#include "../renderer/framebuffer.hpp"
#include <array>

class ShadowMapResources {
public:
    DepthResources* depthResources;
    Sampler* sampler;
    ShadowMapRendererRenderPass* renderPass;
    Framebuffer* framebuffer;
    ShadowMapResources(VkPhysicalDevice& physicalDevice, VkDevice& device, VkExtent2D& extent);
    ~ShadowMapResources();
};

#endif
