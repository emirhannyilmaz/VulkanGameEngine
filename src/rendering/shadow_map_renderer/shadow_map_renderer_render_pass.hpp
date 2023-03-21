#ifndef shadow_map_renderer_render_pass_hpp
#define shadow_map_renderer_render_pass_hpp

#include "../renderer/render_pass.hpp"
#include <array>

class ShadowMapRendererRenderPass : public RenderPass {
public:
    ShadowMapRendererRenderPass(VkDevice& device, VkFormat depthAttachmentFormat);
};

#endif
