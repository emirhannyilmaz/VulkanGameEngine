#ifndef terrain_hpp
#define terrain_hpp

#include "../rendering/mesh.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../rendering/descriptor_pool.hpp"
#include "../rendering/descriptor_set_layout.hpp"
#include "../rendering/descriptor_sets.hpp"
#include "../rendering/renderer_info.hpp"

class Texture;

struct TerrainVertexUniformBufferObject {
    alignas(16) glm::mat4 modelMatrix;
};

struct TerrainFragmentUniformBufferObject {
    alignas(4) float reflectivity;
    alignas(4) float shineDamper;
};

class Terrain {
public:
    Mesh* mesh;
    Texture* texture;
    static DescriptorSetLayout* descriptorSetLayout;
    DescriptorSets* descriptorSets;
    glm::vec2 position;
    Terrain(Texture* texture, glm::vec2 position, Renderer* renderer);
    ~Terrain();
    void updateDescriptorSetResources();
    static void CreateDesriptorSetLayout(VkDevice& device);
    static void DeleteDesriptorSetLayout();
private:
    DescriptorPool* descriptorPool;
    Renderer* renderer;
    std::vector<Buffer*> vertexUniformBuffers;
    std::vector<Buffer*> fragmentUniformBuffers;
    const float SIZE = 100.0f;
    const int VERTEX_COUNT = 128;
    void createMesh();
};

#endif
