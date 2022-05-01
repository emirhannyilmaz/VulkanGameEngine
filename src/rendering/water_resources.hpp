#ifndef water_resources_hpp
#define water_resources_hpp

#include "color_resources.hpp"
#include "depth_resources.hpp"
#include "sampler.hpp"
#include "render_pass.hpp"
#include "framebuffer.hpp"
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
    WaterResources(VkPhysicalDevice& physicalDevice, VkDevice& device, VkExtent2D& extent, VkFormat colorFormat, VkCommandPool& commandPool, VkQueue& graphicsQueue);
    ~WaterResources();
};

#endif
