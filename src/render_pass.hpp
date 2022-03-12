#ifndef render_pass_hpp
#define render_pass_hpp

#include <vulkan/vulkan.h>
#include <stdexcept>

class RenderPass {
public:
    VkRenderPass renderPass;
    RenderPass(VkDevice& device, VkFormat swapchainImageFormat);
    ~RenderPass();
private:
    VkDevice device;
};

#endif
