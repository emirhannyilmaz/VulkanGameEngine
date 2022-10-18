#ifndef water_resources_hpp
#define water_resources_hpp

#include "../renderer/color_resources.hpp"
#include "../renderer/depth_resources.hpp"
#include "../renderer/sampler.hpp"
#include "../renderer/render_pass.hpp"
#include "../renderer/framebuffer.hpp"
#include <array>

class WaterResources {
public:
    ColorResources* reflectionColorResources;
    DepthResources* reflectionDepthResources;
    ColorResources* refractionColorResources;
    DepthResources* refractionDepthResources;
    Sampler* sampler;
    RenderPass* renderPass;
    Framebuffer* reflectionFramebuffer;
    Framebuffer* refractionFramebuffer;
    WaterResources(VkPhysicalDevice& physicalDevice, VkDevice& device, VkExtent2D& extent, VkFormat colorAttachmentFormat);
    ~WaterResources();
};

#endif
