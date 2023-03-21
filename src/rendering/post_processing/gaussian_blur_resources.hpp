#ifndef gaussian_blur_resources_hpp
#define gaussian_blur_resources_hpp

#include "../renderer/color_resources.hpp"
#include "../renderer/depth_resources.hpp"
#include "../renderer/sampler.hpp"
#include "gaussian_blur_raw_render_pass.hpp"
#include "gaussian_blur_horizontal_blur_render_pass.hpp"
#include "../renderer/framebuffer.hpp"
#include <array>

class GaussianBlurResources {
public:
    ColorResources* rawColorResources;
    DepthResources* rawDepthResources;
    ColorResources* horizontalBlurColorResources;
    Sampler* sampler;
    GaussianBlurRawRenderPass* rawRenderPass;
    GaussianBlurHorizontalBlurRenderPass* horizontalBlurRenderPass;
    Framebuffer* rawFramebuffer;
    Framebuffer* horizontalBlurFramebuffer;
    GaussianBlurResources(VkPhysicalDevice& physicalDevice, VkDevice& device, VkExtent2D& extent, VkFormat colorAttachmentFormat);
    ~GaussianBlurResources();
};

#endif
