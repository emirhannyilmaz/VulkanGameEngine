#ifndef entity_renderer_hpp
#define entity_renderer_hpp

#include "renderer.hpp"
#include "../entities/entity.hpp"
#include "../entities/light.hpp"
#include "graphics_pipeline.hpp"
#include "descriptor_set_layout.hpp"
#include "descriptor_pool.hpp"
#include "descriptor_sets.hpp"
#include "buffer.hpp"

struct EntityRendererVertexUniformBufferObject {
    alignas(16) glm::mat4 projectionMatrix;
    alignas(16) glm::vec3 lightPosition;
};

struct EntityRendererFragmentUniformBufferObject {
    alignas(16) glm::vec3 lightColor;
};

struct EntityRendererVertexPushConstants {
    alignas(16) glm::mat4 viewMatrix;
    alignas(16) glm::vec4 clipPlane;
};

class EntityRenderer {
public:
    EntityRenderer(Renderer* renderer);
    ~EntityRenderer();
    void CreateGraphicsPipelines();
    void DeleteGraphicsPipelines();
    void render(std::vector<Entity*> entities, Light* light, PerspectiveCamera* perspectiveCamera, glm::vec4 clipPlane, CommandBuffers* commandBuffers, bool onScreen);
private:
    Renderer* renderer;
    DescriptorSetLayout* descriptorSetLayout;
    GraphicsPipeline* graphicsPipeline;
    GraphicsPipeline* offScreenGraphicsPipeline;
    DescriptorPool* descriptorPool;
    DescriptorSets* descriptorSets;
    std::vector<Buffer*> vertexUniformBuffers;
    std::vector<Buffer*> fragmentUniformBuffers;
    EntityRendererVertexPushConstants vertexPushConstants{};
    void updateDescriptorSetResources(Light* light, PerspectiveCamera* perspectiveCamera);
    void updatePushConstants(PerspectiveCamera* perspectiveCamera, glm::vec4 clipPlane);
};

#endif
