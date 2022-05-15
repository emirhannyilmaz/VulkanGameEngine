#ifndef skybox_renderer_hpp
#define skybox_renderer_hpp

#include "renderer.hpp"
#include "../entities/skybox.hpp"
#include "graphics_pipeline.hpp"
#include "descriptor_set_layout.hpp"
#include "descriptor_pool.hpp"
#include "descriptor_sets.hpp"
#include "buffer.hpp"

struct SkyboxRendererVertexUniformBufferObject {
    alignas(16) glm::mat4 projectionMatrix;
};

struct SkyboxRendererVertexPushConstants {
    alignas(16) glm::mat4 viewMatrix;
    alignas(16) glm::vec4 clipPlane;
};

class SkyboxRenderer {
public:
    SkyboxRenderer(Renderer* renderer);
    ~SkyboxRenderer();
    void CreateGraphicsPipelines();
    void DeleteGraphicsPipelines();
    void render(Skybox* skybox, PerspectiveCamera* perspectiveCamera, glm::vec4 clipPlane, CommandBuffers* commandBuffers, bool onScreen);
private:
    Renderer* renderer;
    DescriptorSetLayout* descriptorSetLayout;
    GraphicsPipeline* graphicsPipeline;
    GraphicsPipeline* offScreenGraphicsPipeline;
    DescriptorPool* descriptorPool;
    DescriptorSets* descriptorSets;
    std::vector<Buffer*> vertexUniformBuffers;
    SkyboxRendererVertexPushConstants vertexPushConstants{};
    void updateDescriptorSetResources(PerspectiveCamera* perspectiveCamera);
    void updatePushConstants(PerspectiveCamera* perspectiveCamera, glm::vec4 clipPlane);
};

#endif
