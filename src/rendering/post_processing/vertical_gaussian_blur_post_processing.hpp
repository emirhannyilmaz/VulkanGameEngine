#ifndef vertical_gaussian_blur_post_processing_hpp
#define vertical_gaussian_blur_post_processing_hpp

#include "../renderer/renderer.hpp"
#include "../renderer/graphics_pipeline.hpp"
#include "../renderer/descriptor_set_layout.hpp"
#include "../renderer/descriptor_pool.hpp"
#include "../renderer/descriptor_sets.hpp"
#include "../renderer/texture.hpp"
#include "../renderer/buffer.hpp"

struct VerticalGaussianBlurVertexUniformBufferObject {
    alignas(4) float imageHeight;
};

class VerticalGaussianBlurPostProcessing {
public:
    VerticalGaussianBlurPostProcessing(Renderer* renderer);
    ~VerticalGaussianBlurPostProcessing();
    void CreateGraphicsPipeline();
    void DeleteGraphicsPipeline();
    void process(float imageHeight, CommandBuffers* commandBuffers);
    void updateDescriptorSetImageInfos();
private:
    Renderer* renderer;
    DescriptorSetLayout* descriptorSetLayout;
    GraphicsPipeline* graphicsPipeline;
    DescriptorPool* descriptorPool;
    DescriptorSets* descriptorSets;
    std::vector<Buffer*> vertexUniformBuffers;
    void updateDescriptorSetResources(float imageHeight);
};

#endif
