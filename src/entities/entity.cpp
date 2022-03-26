#include "entity.hpp"

Entity::Entity(Mesh* mesh, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
    this->mesh = mesh;
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
}

Entity::~Entity() {
    delete mesh;
}

glm::mat4 Entity::createModelMatrix() {
    glm::mat4 matrix(1.0f);
    matrix = glm::translate(matrix, position);
    matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    matrix = glm::scale(matrix, scale);

    return matrix;
}
