#ifndef gaussian_blur_raw_render_pass_hpp
#define gaussian_blur_raw_render_pass_hpp

#include "../renderer/render_pass.hpp"
#include <array>

class GaussianBlurRawRenderPass : public RenderPass {
public:
    GaussianBlurRawRenderPass(VkDevice& device, VkFormat colorAttachmentFormat, VkFormat depthAttachmentFormat);
};

#endif
