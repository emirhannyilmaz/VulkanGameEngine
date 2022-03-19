#ifndef entity_hpp
#define entity_hpp

#include "../rendering/mesh.hpp"
#include <glm/glm.hpp>

class Entity {
public:
    Mesh* mesh;
    glm::vec3 position;
    float rotation;
    glm::vec3 rotationAxis;
    glm::vec3 scale;
    Entity(Mesh* mesh, glm::vec3 position, float rotation, glm::vec3 rotationAxis, glm::vec3 scale);
    ~Entity();
};

#endif
