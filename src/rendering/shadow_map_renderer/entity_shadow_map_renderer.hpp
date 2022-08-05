#ifndef entity_shadow_map_renderer_hpp
#define entity_shadow_map_renderer_hpp

#include "../renderer/renderer.hpp"
#include "../../entities/entity.hpp"
#include "../../entities/light.hpp"
#include "../../entities/orthographic_camera.hpp"
#include "../renderer/graphics_pipeline.hpp"
#include "../renderer/descriptor_set_layout.hpp"
#include "../renderer/descriptor_pool.hpp"
#include "../renderer/descriptor_sets.hpp"
#include "../renderer/buffer.hpp"

struct EntityShadowMapRendererVertexUniformBufferObject {
    alignas(16) glm::mat4 viewMatrix;
    alignas(16) glm::mat4 projectionMatrix;
};

class EntityShadowMapRenderer {
public:
    EntityShadowMapRenderer(Renderer* renderer);
    ~EntityShadowMapRenderer();
    void CreateGraphicsPipeline();
    void DeleteGraphicsPipeline();
    void render(std::vector<Entity*> entities, Light* light, PerspectiveCamera* perspectiveCamera, OrthographicCamera* orthographicCamera, CommandBuffers* commandBuffers);
private:
    Renderer* renderer;
    DescriptorSetLayout* descriptorSetLayout;
    GraphicsPipeline* graphicsPipeline;
    DescriptorPool* descriptorPool;
    DescriptorSets* descriptorSets;
    std::vector<Buffer*> vertexUniformBuffers;
    void updateDescriptorSetResources(Light* light, PerspectiveCamera* perspectiveCamera, OrthographicCamera* orthographicCamera);
};

#endif
