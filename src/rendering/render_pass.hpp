#ifndef render_pass_hpp
#define render_pass_hpp

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <array>

class RenderPass {
public:
    VkRenderPass renderPass;
    RenderPass(VkDevice& device, VkFormat colorAttachmentFormat, VkFormat depthAttachmentFormat, VkSampleCountFlagBits msaaSamples);
    ~RenderPass();
private:
    VkDevice device;
};

#endif
