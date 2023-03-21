#ifndef skybox_renderer_hpp
#define skybox_renderer_hpp

#include "../renderer/renderer.hpp"
#include "../../entities/skybox.hpp"
#include "../renderer/graphics_pipeline.hpp"
#include "../renderer/descriptor_set_layout.hpp"
#include "../renderer/descriptor_pool.hpp"
#include "../renderer/descriptor_sets.hpp"
#include "../renderer/buffer.hpp"

struct SkyboxRendererVertexUniformBufferObject {
    alignas(16) glm::mat4 projectionMatrix;
};

struct SkyboxRendererFragmentUniformBufferObject {
    alignas(16) glm::vec3 fogColor;
};

struct SkyboxRendererVertexPushConstants {
    alignas(16) glm::mat4 viewMatrix;
    alignas(16) glm::vec4 clipPlane;
};

class SkyboxRenderer {
public:
    GraphicsPipeline* graphicsPipeline;
    GraphicsPipeline* graphicsPipelineForWater;
    GraphicsPipeline* graphicsPipelineForGaussianBlur;
    SkyboxRenderer(Renderer* renderer);
    ~SkyboxRenderer();
    void CreateGraphicsPipelines();
    void DeleteGraphicsPipelines();
    void render(Skybox* skybox, PerspectiveCamera* perspectiveCamera, glm::vec4 clipPlane, CommandBuffers* commandBuffers, GraphicsPipeline* graphicsPipeline);
private:
    Renderer* renderer;
    DescriptorSetLayout* descriptorSetLayout;
    DescriptorPool* descriptorPool;
    DescriptorSets* descriptorSets;
    std::vector<Buffer*> vertexUniformBuffers;
    std::vector<Buffer*> fragmentUniformBuffers;
    SkyboxRendererVertexPushConstants vertexPushConstants{};
    void updateDescriptorSetResources(PerspectiveCamera* perspectiveCamera);
    void updatePushConstants(PerspectiveCamera* perspectiveCamera, glm::vec4 clipPlane);
};

#endif
