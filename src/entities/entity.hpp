#ifndef entity_hpp
#define entity_hpp

#include "../rendering/mesh.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../rendering/descriptor_pool.hpp"
#include "../rendering/descriptor_layout.hpp"
#include "../rendering/descriptor_sets.hpp"

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
    DescriptorSets* descriptorSets;
    std::vector<Buffer*> uniformBuffers;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    Entity(Mesh* mesh, Texture* texture, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    ~Entity();
    glm::mat4 createModelMatrix();
private:
    DescriptorPool* descriptorPool;
    DescriptorLayout* descriptorSetLayout;
};

#endif
