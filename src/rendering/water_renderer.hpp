#ifndef water_renderer_hpp
#define water_renderer_hpp

#include "renderer.hpp"
#include "../entities/water_tile.hpp"
#include "graphics_pipeline.hpp"
#include "descriptor_set_layout.hpp"
#include "descriptor_pool.hpp"
#include "descriptor_sets.hpp"
#include "buffer.hpp"

struct WaterRendererVertexUniformBufferObject {
    alignas(16) glm::mat4 viewMatrix;
    alignas(16) glm::mat4 projectionMatrix;
};

class WaterRenderer {
public:
    WaterRenderer(Renderer* renderer);
    ~WaterRenderer();
    void render(std::vector<WaterTile*> waterTiles, Camera* camera, CommandBuffers* commandBuffers);
    void updateDescriptorSetImageInfos();
private:
    Renderer* renderer;
    GraphicsPipeline* graphicsPipeline;
    DescriptorSetLayout* descriptorSetLayout;
    DescriptorPool* descriptorPool;
    DescriptorSets* descriptorSets;
    std::vector<Buffer*> vertexUniformBuffers;
    void updateDescriptorSetResources(Camera* camera);
};

#endif
