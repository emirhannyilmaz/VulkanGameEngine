#ifndef render_pass_hpp
#define render_pass_hpp

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>

class RenderPass {
public:
    VkRenderPass renderPass;
    RenderPass(VkDevice& device);
    ~RenderPass();
private:
    VkDevice device;
};

#endif
