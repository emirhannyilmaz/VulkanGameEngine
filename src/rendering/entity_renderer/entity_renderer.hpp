#ifndef entity_renderer_hpp
#define entity_renderer_hpp

#include "../renderer/renderer.hpp"
#include "../../entities/entity.hpp"
#include "../../entities/light.hpp"
#include "../renderer/graphics_pipeline.hpp"
#include "../renderer/descriptor_set_layout.hpp"
#include "../renderer/descriptor_pool.hpp"
#include "../renderer/descriptor_sets.hpp"
#include "../renderer/buffer.hpp"

struct EntityRendererVertexUniformBufferObject {
    alignas(16) glm::mat4 projectionMatrix;
    alignas(16) glm::vec3 lightPosition;
};

struct EntityRendererFragmentUniformBufferObject {
    alignas(16) glm::vec3 lightColor;
    alignas(16) glm::vec3 fogColor;
};

struct EntityRendererVertexPushConstants {
    alignas(16) glm::mat4 viewMatrix;
    alignas(16) glm::vec4 clipPlane;
};

class EntityRenderer {
public:
    GraphicsPipeline* graphicsPipeline;
    GraphicsPipeline* graphicsPipelineForWater;
    GraphicsPipeline* graphicsPipelineForGaussianBlur;
    EntityRenderer(Renderer* renderer);
    ~EntityRenderer();
    void CreateGraphicsPipelines();
    void DeleteGraphicsPipelines();
    void render(std::vector<Entity*> entities, Light* light, PerspectiveCamera* perspectiveCamera, glm::vec4 clipPlane, CommandBuffers* commandBuffers, GraphicsPipeline* graphicsPipeline);
private:
    Renderer* renderer;
    DescriptorSetLayout* descriptorSetLayout;
    DescriptorPool* descriptorPool;
    DescriptorSets* descriptorSets;
    std::vector<Buffer*> vertexUniformBuffers;
    std::vector<Buffer*> fragmentUniformBuffers;
    EntityRendererVertexPushConstants vertexPushConstants{};
    void updateDescriptorSetResources(Light* light, PerspectiveCamera* perspectiveCamera);
    void updatePushConstants(PerspectiveCamera* perspectiveCamera, glm::vec4 clipPlane);
};

#endif
