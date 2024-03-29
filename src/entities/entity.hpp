#ifndef entity_hpp
#define entity_hpp

#include "../rendering/entity_renderer/entity_mesh.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../rendering/renderer/descriptor_pool.hpp"
#include "../rendering/renderer/descriptor_set_layout.hpp"
#include "../rendering/renderer/descriptor_sets.hpp"
#include "../rendering/renderer/common.hpp"

class Texture;

struct EntityVertexUniformBufferObject {
    alignas(16) glm::mat4 modelMatrix;
};

struct EntityFragmentUniformBufferObject {
    alignas(4) float reflectivity;
    alignas(4) float shineDamper;
};

class Entity {
public:
    EntityMesh* mesh;
    Texture* texture;
    static DescriptorSetLayout* descriptorSetLayout;
    DescriptorSets* descriptorSets;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    Entity(EntityMesh* mesh, Texture* texture, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Renderer* renderer);
    ~Entity();
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
