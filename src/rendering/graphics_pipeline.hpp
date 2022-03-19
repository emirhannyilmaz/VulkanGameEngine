#ifndef graphics_pipeline_hpp
#define graphics_pipeline_hpp

#include <vulkan/vulkan.h>
#include "shader_module.hpp"
#include <stdexcept>
#include "vertex.hpp"

class GraphicsPipeline {
public:
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    GraphicsPipeline(VkDevice& device, VkDescriptorSetLayout& descriptorSetLayout, VkExtent2D& swapchainExtent, VkRenderPass& renderPass);
    ~GraphicsPipeline();
private:
    VkDevice device;
};

#endif
