#ifndef default_render_pass_hpp
#define default_render_pass_hpp

#include "render_pass.hpp"
#include <array>

class DefaultRenderPass : public RenderPass {
public:
    DefaultRenderPass(VkDevice& device, VkFormat colorAttachmentFormat, VkFormat depthAttachmentFormat, VkSampleCountFlagBits msaaSamples);
};

#endif
