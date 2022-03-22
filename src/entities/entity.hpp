#ifndef entity_hpp
#define entity_hpp

#include "../rendering/mesh.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Entity {
public:
    Mesh* mesh;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    Entity(Mesh* mesh, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    ~Entity();
    glm::mat4 createModelMatrix();
};

#endif
