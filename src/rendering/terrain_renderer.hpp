#ifndef terrain_renderer_hpp
#define terrain_renderer_hpp

#include "renderer.hpp"
#include "../entities/terrain.hpp"
#include "../entities/light.hpp"
#include "graphics_pipeline.hpp"
#include "descriptor_set_layout.hpp"
#include "descriptor_pool.hpp"
#include "descriptor_sets.hpp"
#include "buffer.hpp"

struct TerrainRendererVertexUniformBufferObject {
    alignas(16) glm::mat4 projectionMatrix;
    alignas(16) glm::vec3 lightPosition;
};

struct TerrainRendererFragmentUniformBufferObject {
    alignas(16) glm::vec3 lightColor;
};

struct TerrainRendererVertexPushConstants {
    alignas(16) glm::mat4 viewMatrix;
    alignas(16) glm::vec4 clipPlane;
};

class TerrainRenderer {
public:
    TerrainRenderer(Renderer* renderer);
    ~TerrainRenderer();
    void CreateGraphicsPipelines();
    void DeleteGraphicsPipelines();
    void render(std::vector<Terrain*> terrains, Light* light, Camera* camera, glm::vec4 clipPlane, CommandBuffers* commandBuffers, bool onScreen);
private:
    Renderer* renderer;
    DescriptorSetLayout* descriptorSetLayout;
    GraphicsPipeline* graphicsPipeline;
    GraphicsPipeline* offScreenGraphicsPipeline;
    DescriptorPool* descriptorPool;
    DescriptorSets* descriptorSets;
    std::vector<Buffer*> vertexUniformBuffers;
    std::vector<Buffer*> fragmentUniformBuffers;
    TerrainRendererVertexPushConstants vertexPushConstants{};
    void updateDescriptorSetResources(Light* light, Camera* camera);
    void updatePushConstants(Camera* camera, glm::vec4 clipPlane);
};

#endif
