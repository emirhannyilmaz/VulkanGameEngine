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
    GraphicsPipeline(VkDevice& device, const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName, uint32_t vertexBindingDescriptionCount, VkVertexInputBindingDescription* vertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, VkVertexInputAttributeDescription* vertexAttributeDescriptions, uint32_t descriptorSetLayoutCount, VkDescriptorSetLayout* descriptorSetLayouts, VkExtent2D& swapchainExtent, VkRenderPass& renderPass, VkSampleCountFlagBits msaaSamples);
    ~GraphicsPipeline();
private:
    VkDevice device;
};

#endif
