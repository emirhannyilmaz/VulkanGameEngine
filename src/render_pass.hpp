#ifndef render_pass_hpp
#define render_pass_hpp

#define VK_ENABLE_BETA_EXTENSIONS
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class RenderPass {
public:
    VkRenderPass renderPass;
    RenderPass(VkDevice device, VkFormat swapchainImageFormat);
    ~RenderPass();
private:
    VkDevice device;
};
