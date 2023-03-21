#ifndef water_renderer_render_pass_hpp
#define water_renderer_render_pass_hpp

#include "../renderer/render_pass.hpp"
#include <array>

class WaterRendererRenderPass : public RenderPass {
public:
    WaterRendererRenderPass(VkDevice& device, VkFormat colorAttachmentFormat, VkFormat depthAttachmentFormat);
};

#endif
