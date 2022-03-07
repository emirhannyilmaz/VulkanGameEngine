#ifndef framebuffer_hpp
#define framebuffer_hpp

#include <vulkan/vulkan.h>
#include <stdexcept>

class Framebuffer {
public:
    VkFramebuffer framebuffer;
    Framebuffer(VkDevice device, VkRenderPass renderPass, VkImageView* attachments, VkExtent2D extent);
    ~Framebuffer();
private:
    VkDevice device;
};

#endif
