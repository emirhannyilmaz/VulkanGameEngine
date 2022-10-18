#ifndef render_pass_hpp
#define render_pass_hpp

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>

class RenderPass {
public:
    VkRenderPass renderPass;
    RenderPass(VkDevice& device, VkFormat colorAttachmentFormat, VkFormat depthAttachmentFormat, VkSampleCountFlagBits msaaSamples, bool onScreen, bool hasColorAttachment, bool hasDepthAttachment, bool hasColorAttachmentResolve);
    ~RenderPass();
private:
    VkDevice device;
};

#endif
