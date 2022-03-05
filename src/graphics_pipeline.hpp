#ifndef graphics_pipeline_hpp
#define graphics_pipeline_hpp

#define VK_ENABLE_BETA_EXTENSIONS
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "shader_module.hpp"

class GraphicsPipeline {
public:
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    GraphicsPipeline(VkDevice device, VkExtent2D swapchainExtent);
    ~GraphicsPipeline();
private:
    VkDevice device;
};

#endif
