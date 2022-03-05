#ifndef framebuffer_hpp
#define framebuffer_hpp

#define VK_ENABLE_BETA_EXTENSIONS
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Framebuffer {
public:
    VkFramebuffer framebuffer;
    Framebuffer(VkDevice device, VkRenderPass renderPass, VkImageView* attachments, VkExtent2D extent);
    ~Framebuffer();
private:
    VkDevice device;
};

#endif
