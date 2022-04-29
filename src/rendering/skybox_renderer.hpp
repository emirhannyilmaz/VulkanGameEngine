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
    alignas(16) glm::mat4 viewMatrix;
    alignas(16) glm::mat4 projectionMatrix;
};

class SkyboxRenderer {
public:
    SkyboxRenderer(Renderer* renderer);
    ~SkyboxRenderer();
    void render(Skybox* skybox, Camera* camera, CommandBuffers* commandBuffers, bool onScreen);
private:
    Renderer* renderer;
    GraphicsPipeline* graphicsPipeline;
    GraphicsPipeline* offScreenGraphicsPipeline;
    DescriptorSetLayout* descriptorSetLayout;
    DescriptorPool* descriptorPool;
    DescriptorSets* descriptorSets;
    std::vector<Buffer*> vertexUniformBuffers;
    void updateDescriptorSetResources(Camera* camera);
};

#endif
