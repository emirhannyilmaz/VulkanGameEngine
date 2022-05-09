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
};

struct WaterTileFragmentUniformBufferObject {
    alignas(4) float reflectivity;
    alignas(4) float shineDamper;
};

class WaterTile {
public:
    glm::vec3 position;
    glm::vec2 scale;
    float reflectivity;
    float shineDamper;
    static DescriptorSetLayout* descriptorSetLayout;
    DescriptorSets* descriptorSets;
    WaterTile(glm::vec3 position, glm::vec2 scale, float reflectivity, float shineDamper, Renderer* renderer);
    ~WaterTile();
    void updateDescriptorSetResources();
    static void CreateDesriptorSetLayout(VkDevice& device);
    static void DeleteDesriptorSetLayout();
private:
    DescriptorPool* descriptorPool;
    Renderer* renderer;
    std::vector<Buffer*> vertexUniformBuffers;
    std::vector<Buffer*> fragmentUniformBuffers;
};

#endif
