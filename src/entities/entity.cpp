#include "entity.hpp"

Entity::Entity(Mesh* mesh, glm::vec3 position, float rotation, glm::vec3 rotationAxis, glm::vec3 scale) {
    this->mesh = mesh;
    this->position = position;
    this->rotation = rotation;
    this->rotationAxis = rotationAxis;
    this->scale = scale;
}

Entity::~Entity() {
    delete mesh;
}
