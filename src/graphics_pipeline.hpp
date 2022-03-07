#ifndef graphics_pipeline_hpp
#define graphics_pipeline_hpp

#include <vulkan/vulkan.h>
#include "shader_module.hpp"
#include <stdexcept>

class GraphicsPipeline {
public:
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    GraphicsPipeline(VkDevice device, VkExtent2D swapchainExtent, VkRenderPass renderPass);
    ~GraphicsPipeline();
private:
    VkDevice device;
};

#endif
