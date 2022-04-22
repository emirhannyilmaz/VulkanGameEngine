#ifndef water_tile_hpp
#define water_tile_hpp

#include <glm/glm.hpp>
#include "../rendering/descriptor_pool.hpp"
#include "../rendering/descriptor_set_layout.hpp"
#include "../rendering/descriptor_sets.hpp"
#include "../rendering/renderer_info.hpp"
#include "../rendering/renderer.hpp"
#include "../rendering/buffer.hpp"

struct WaterTileVertexUniformBufferObject {
    alignas(16) glm::mat4 modelMatrix;
    alignas(16) glm::mat4 viewMatrix;
    alignas(16) glm::mat4 projectionMatrix;
};

class WaterTile {
public:
    glm::vec3 position;
    glm::vec2 scale;
    static DescriptorSetLayout* descriptorSetLayout;
    DescriptorSets* descriptorSets;
    WaterTile(glm::vec3 position, glm::vec2 scale, Renderer* renderer);
    ~WaterTile();
    void updateDescriptorSetResources();
    static void CreateDesriptorSetLayout(VkDevice& device);
    static void DeleteDesriptorSetLayout();
private:
    DescriptorPool* descriptorPool;
    Renderer* renderer;
    std::vector<Buffer*> vertexUniformBuffers;
};

#endif
