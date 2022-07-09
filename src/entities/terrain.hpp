#ifndef terrain_hpp
#define terrain_hpp

#include "../rendering/terrain_renderer/terrain_mesh.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../rendering/renderer/descriptor_pool.hpp"
#include "../rendering/renderer/descriptor_set_layout.hpp"
#include "../rendering/renderer/descriptor_sets.hpp"
#include "../rendering/renderer/renderer_info.hpp"
#include <stb_image.h>

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
    TerrainMesh* mesh;
    Texture* texture;
    static DescriptorSetLayout* descriptorSetLayout;
    DescriptorSets* descriptorSets;
    glm::vec2 position;
    Terrain(Texture* texture, const std::string& heightMapFileName, glm::vec2 position, Renderer* renderer);
    ~Terrain();
    void updateDescriptorSetResources();
    static void CreateDesriptorSetLayout(VkDevice& device);
    static void DeleteDesriptorSetLayout();
    float getHeightOfTerrain(float x, float z);
private:
    DescriptorPool* descriptorPool;
    Renderer* renderer;
    std::vector<Buffer*> vertexUniformBuffers;
    std::vector<Buffer*> fragmentUniformBuffers;
    const float SIZE = 800.0f;
    const float MAX_HEIGHT = 80.0f;
    std::vector<std::vector<glm::vec3>> vertexPositions;
    void createMesh(const std::string& heightMapFileName);
    float getHeight(int x, int z, int width, int height, stbi_uc* heightMapPixels);
    glm::vec3 getNormal(int x, int z, int width, int height, stbi_uc* heightMapPixels);
    float interpolateHeight(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC, float x, float z);
};

#endif
