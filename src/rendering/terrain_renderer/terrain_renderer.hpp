#ifndef terrain_renderer_hpp
#define terrain_renderer_hpp

#include "../renderer/renderer.hpp"
#include "../../entities/terrain.hpp"
#include "../../entities/light.hpp"
#include "../../entities/orthographic_camera.hpp"
#include "../renderer/graphics_pipeline.hpp"
#include "../renderer/descriptor_set_layout.hpp"
#include "../renderer/descriptor_pool.hpp"
#include "../renderer/descriptor_sets.hpp"
#include "../renderer/buffer.hpp"

struct TerrainRendererVertexUniformBufferObject {
    alignas(16) glm::mat4 projectionMatrix;
    alignas(16) glm::vec3 lightPosition;
    alignas(16) glm::mat4 toShadowMapSpaceMatrix;
    alignas(4) float shadowDistance;
};

struct TerrainRendererFragmentUniformBufferObject {
    alignas(16) glm::vec3 lightColor;
    alignas(4) float shadowMapSize;
    alignas(16) glm::vec3 fogColor;
};

struct TerrainRendererVertexPushConstants {
    alignas(16) glm::mat4 viewMatrix;
    alignas(16) glm::vec4 clipPlane;
};

class TerrainRenderer {
public:
    GraphicsPipeline* graphicsPipeline;
    GraphicsPipeline* graphicsPipelineForWater;
    GraphicsPipeline* graphicsPipelineForGaussianBlur;
    TerrainRenderer(Renderer* renderer);
    ~TerrainRenderer();
    void CreateGraphicsPipelines();
    void DeleteGraphicsPipelines();
    void render(std::vector<Terrain*> terrains, Light* light, PerspectiveCamera* perspectiveCamera, OrthographicCamera* orthographicCamera, glm::vec4 clipPlane, CommandBuffers* commandBuffers, GraphicsPipeline* graphicsPipeline);
    void updateDescriptorSetImageInfos();
private:
    Renderer* renderer;
    DescriptorSetLayout* descriptorSetLayout;
    DescriptorPool* descriptorPool;
    DescriptorSets* descriptorSets;
    std::vector<Buffer*> vertexUniformBuffers;
    std::vector<Buffer*> fragmentUniformBuffers;
    TerrainRendererVertexPushConstants vertexPushConstants{};
    void updateDescriptorSetResources(Light* light, PerspectiveCamera* perspectiveCamera, OrthographicCamera* orthographicCamera);
    void updatePushConstants(PerspectiveCamera* perspectiveCamera, glm::vec4 clipPlane);
    glm::mat4 getToShadowMapSpaceMatrix(Light* light, PerspectiveCamera* perspectiveCamera, OrthographicCamera* orthographicCamera);
};

#endif
