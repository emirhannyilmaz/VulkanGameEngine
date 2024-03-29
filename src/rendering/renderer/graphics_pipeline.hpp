#ifndef graphics_pipeline_hpp
#define graphics_pipeline_hpp

#include <vulkan/vulkan.h>
#include "shader_module.hpp"
#include <stdexcept>

class GraphicsPipeline {
public:
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    GraphicsPipeline(VkDevice& device, const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName, uint32_t vertexBindingDescriptionCount, VkVertexInputBindingDescription* vertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, VkVertexInputAttributeDescription* vertexAttributeDescriptions, uint32_t descriptorSetLayoutCount, VkDescriptorSetLayout* descriptorSetLayouts, uint32_t pushConstantRangeCount, VkPushConstantRange* pushConstantRanges, VkExtent2D& swapchainExtent, VkRenderPass& renderPass, VkSampleCountFlagBits msaaSamples, VkBool32 depthWriteEnable, VkBlendFactor dstColorBlendFactor);
    ~GraphicsPipeline();
private:
    VkDevice device;
};

#endif
