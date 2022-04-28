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
    alignas(16) glm::mat4 viewMatrix;
    alignas(16) glm::mat4 projectionMatrix;
    alignas(16) glm::vec3 lightPosition;
    alignas(16) glm::vec4 clipPlane;
};

struct EntityRendererFragmentUniformBufferObject {
    alignas(16) glm::vec3 lightColor;
};

class EntityRenderer {
public:
    EntityRenderer(Renderer* renderer);
    ~EntityRenderer();
    void render(std::vector<Entity*> entities, Light* light, Camera* camera, glm::vec4 clipPlane, CommandBuffers* commandBuffers);
private:
    Renderer* renderer;
    GraphicsPipeline* graphicsPipeline;
    DescriptorSetLayout* descriptorSetLayout;
    DescriptorPool* descriptorPool;
    DescriptorSets* descriptorSets;
    std::vector<Buffer*> vertexUniformBuffers;
    std::vector<Buffer*> fragmentUniformBuffers;
    void updateDescriptorSetResources(Light* light, Camera* camera, glm::vec4 clipPlane);
};

#endif
