#ifndef animated_entity_renderer_hpp
#define animated_entity_renderer_hpp

#include "../renderer/renderer.hpp"
#include "../../entities/animated_entity.hpp"
#include "../../entities/light.hpp"
#include "../renderer/graphics_pipeline.hpp"
#include "../renderer/descriptor_set_layout.hpp"
#include "../renderer/descriptor_pool.hpp"
#include "../renderer/descriptor_sets.hpp"
#include "../renderer/buffer.hpp"

struct AnimatedEntityRendererVertexUniformBufferObject {
    alignas(16) glm::mat4 projectionMatrix;
    alignas(16) glm::vec3 lightPosition;
};

struct AnimatedEntityRendererFragmentUniformBufferObject {
    alignas(16) glm::vec3 lightColor;
    alignas(16) glm::vec3 fogColor;
};

struct AnimatedEntityRendererVertexPushConstants {
    alignas(16) glm::mat4 viewMatrix;
    alignas(16) glm::vec4 clipPlane;
};

class AnimatedEntityRenderer {
public:
    AnimatedEntityRenderer(Renderer* renderer);
    ~AnimatedEntityRenderer();
    void CreateGraphicsPipelines();
    void DeleteGraphicsPipelines();
    void render(std::vector<AnimatedEntity*> animatedEntities, Light* light, PerspectiveCamera* perspectiveCamera, glm::vec4 clipPlane, CommandBuffers* commandBuffers, bool onScreen);
private:
    Renderer* renderer;
    DescriptorSetLayout* descriptorSetLayout;
    GraphicsPipeline* graphicsPipeline;
    GraphicsPipeline* offScreenGraphicsPipeline;
    DescriptorPool* descriptorPool;
    DescriptorSets* descriptorSets;
    std::vector<Buffer*> vertexUniformBuffers;
    std::vector<Buffer*> fragmentUniformBuffers;
    AnimatedEntityRendererVertexPushConstants vertexPushConstants{};
    void updateDescriptorSetResources(Light* light, PerspectiveCamera* perspectiveCamera);
    void updatePushConstants(PerspectiveCamera* perspectiveCamera, glm::vec4 clipPlane);
};

#endif
