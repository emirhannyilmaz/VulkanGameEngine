#ifndef water_resources_hpp
#define water_resources_hpp

#include "color_resources.hpp"
#include "depth_resources.hpp"
#include "render_pass.hpp"
#include "framebuffer.hpp"
#include <array>

class WaterResources {
public:
    RenderPass* renderPass;
    Framebuffer* reflectionFramebuffer;
    Framebuffer* refractionFramebuffer;
    WaterResources(VkPhysicalDevice& physicalDevice, VkDevice& device, VkExtent2D& extent, VkSampleCountFlagBits msaaSamples, VkFormat colorFormat, VkCommandPool& commandPool, VkQueue& graphicsQueue);
    ~WaterResources();
private:
    ColorResources* colorResources;
    DepthResources* depthResources;
};

#endif
