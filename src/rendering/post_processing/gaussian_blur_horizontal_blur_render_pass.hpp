#ifndef gaussian_blur_horizontal_blur_render_pass_hpp
#define gaussian_blur_horizontal_blur_render_pass_hpp

#include "../renderer/render_pass.hpp"
#include <array>

class GaussianBlurHorizontalBlurRenderPass : public RenderPass {
public:
    GaussianBlurHorizontalBlurRenderPass(VkDevice& device, VkFormat colorAttachmentFormat);
};

#endif
