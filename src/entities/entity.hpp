#ifndef entity_hpp
#define entity_hpp

#include "../rendering/mesh.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../rendering/descriptor_pool.hpp"
#include "../rendering/descriptor_set_layout.hpp"
#include "../rendering/descriptor_sets.hpp"
#include "../rendering/renderer_info.hpp"

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
    Mesh* mesh;
    Texture* texture;
    static DescriptorSetLayout* descriptorSetLayout;
    DescriptorSets* descriptorSets;
    std::vector<Buffer*> vertexUniformBuffers;
    std::vector<Buffer*> fragmentUniformBuffers;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    Entity(Mesh* mesh, Texture* texture, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Renderer* renderer);
    ~Entity();
    void updateDescriptorSetResources(uint32_t currentFrame);
    static void CreateDesriptorSetLayout(VkDevice& device);
    static void DeleteDesriptorSetLayout();
private:
    DescriptorPool* descriptorPool;
    Renderer* renderer;
};

#endif
