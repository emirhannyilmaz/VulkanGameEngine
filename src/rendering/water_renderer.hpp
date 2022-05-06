#ifndef water_renderer_hpp
#define water_renderer_hpp

#include "renderer.hpp"
#include "../entities/water_tile.hpp"
#include "graphics_pipeline.hpp"
#include "descriptor_set_layout.hpp"
#include "descriptor_pool.hpp"
#include "descriptor_sets.hpp"
#include "texture.hpp"
#include "buffer.hpp"

struct WaterRendererVertexUniformBufferObject {
    alignas(16) glm::mat4 viewMatrix;
    alignas(16) glm::mat4 projectionMatrix;
};

struct WaterRendererFragmentUniformBufferObject {
    alignas(4) float moveFactor;
};

class WaterRenderer {
public:
    WaterRenderer(Renderer* renderer);
    ~WaterRenderer();
    void CreateGraphicsPipeline();
    void DeleteGraphicsPipeline();
    void render(std::vector<WaterTile*> waterTiles, Camera* camera, CommandBuffers* commandBuffers);
    void updateDescriptorSetImageInfos();
private:
    Renderer* renderer;
    DescriptorSetLayout* descriptorSetLayout;
    GraphicsPipeline* graphicsPipeline;
    DescriptorPool* descriptorPool;
    DescriptorSets* descriptorSets;
    Texture* dudvMap;
    std::vector<Buffer*> vertexUniformBuffers;
    std::vector<Buffer*> fragmentUniformBuffers;
    const float waveSpeed = 0.03f;
    float moveFactor = 0.0f;
    void updateDescriptorSetResources(Camera* camera);
};

#endif
