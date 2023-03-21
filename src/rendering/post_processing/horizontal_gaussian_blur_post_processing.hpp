#ifndef horizontal_gaussian_blur_post_processing_hpp
#define horizontal_gaussian_blur_post_processing_hpp

#include "../renderer/renderer.hpp"
#include "../renderer/graphics_pipeline.hpp"
#include "../renderer/descriptor_set_layout.hpp"
#include "../renderer/descriptor_pool.hpp"
#include "../renderer/descriptor_sets.hpp"
#include "../renderer/texture.hpp"
#include "../renderer/buffer.hpp"

struct HorizontalGaussianBlurVertexUniformBufferObject {
    alignas(4) float imageWidth;
};

class HorizontalGaussianBlurPostProcessing {
public:
    HorizontalGaussianBlurPostProcessing(Renderer* renderer);
    ~HorizontalGaussianBlurPostProcessing();
    void CreateGraphicsPipeline();
    void DeleteGraphicsPipeline();
    void process(float imageWidth, CommandBuffers* commandBuffers);
    void updateDescriptorSetImageInfos();
private:
    Renderer* renderer;
    DescriptorSetLayout* descriptorSetLayout;
    GraphicsPipeline* graphicsPipeline;
    DescriptorPool* descriptorPool;
    DescriptorSets* descriptorSets;
    std::vector<Buffer*> vertexUniformBuffers;
    void updateDescriptorSetResources(float imageWidth);
};

#endif
