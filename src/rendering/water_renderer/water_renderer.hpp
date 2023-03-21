#ifndef water_renderer_hpp
#define water_renderer_hpp

#include "../renderer/renderer.hpp"
#include "../../entities/water_tile.hpp"
#include "../renderer/graphics_pipeline.hpp"
#include "../renderer/descriptor_set_layout.hpp"
#include "../renderer/descriptor_pool.hpp"
#include "../renderer/descriptor_sets.hpp"
#include "../renderer/texture.hpp"
#include "../renderer/buffer.hpp"
#include "../../entities/light.hpp"

struct WaterRendererVertexUniformBufferObject {
    alignas(16) glm::mat4 viewMatrix;
    alignas(16) glm::mat4 projectionMatrix;
    alignas(16) glm::vec3 lightPosition;
};

struct WaterRendererFragmentUniformBufferObject {
    alignas(4) float moveFactor;
    alignas(16) glm::vec3 lightColor;
    alignas(4) float nearPlane;
    alignas(4) float farPlane;
    alignas(16) glm::vec3 fogColor;
};

class WaterRenderer {
public:
    GraphicsPipeline* graphicsPipeline;
    GraphicsPipeline* graphicsPipelineForGaussianBlur;
    WaterRenderer(Renderer* renderer);
    ~WaterRenderer();
    void CreateGraphicsPipelines();
    void DeleteGraphicsPipelines();
    void render(std::vector<WaterTile*> waterTiles, PerspectiveCamera* perspectiveCamera, Light* light, CommandBuffers* commandBuffers, GraphicsPipeline* graphicsPipeline);
    void updateDescriptorSetImageInfos();
private:
    Renderer* renderer;
    DescriptorSetLayout* descriptorSetLayout;
    DescriptorPool* descriptorPool;
    DescriptorSets* descriptorSets;
    Texture* dudvMap;
    Texture* normalMap;
    std::vector<Buffer*> vertexUniformBuffers;
    std::vector<Buffer*> fragmentUniformBuffers;
    const float waveSpeed = 0.3f;
    float moveFactor = 0.0f;
    void updateDescriptorSetResources(PerspectiveCamera* perspectiveCamera, Light* light);
};

#endif
