#include "light.hpp"

Light::Light(glm::vec3 position, glm::vec3 color) {
    this->position = position;
    this->color = color;
}

void Light::update(float speed, float deltaTime) {
    if (Input::GetKey(GLFW_KEY_UP)) {
        position.z += speed * deltaTime;
    }

    if (Input::GetKey(GLFW_KEY_DOWN)) {
        position.z -= speed * deltaTime;
    }

    if (Input::GetKey(GLFW_KEY_LEFT)) {
        position.x -= speed * deltaTime;
    }

    if (Input::GetKey(GLFW_KEY_RIGHT)) {
        position.x += speed * deltaTime;
    }

    if (Input::GetKey(GLFW_KEY_SPACE)) {
        position.y -= speed * deltaTime;
    }

    if (Input::GetKey(GLFW_KEY_LEFT_SHIFT)) {
        position.y += speed * deltaTime;
    }

    direction = position * -1.0f;
    direction = glm::normalize(direction);
}

glm::mat4 Light::createViewMatrix(glm::vec3 center, float zOffset) {
    viewMatrix = glm::mat4(1.0f);
    float pitch = glm::acos(glm::length(glm::vec2(direction.x, direction.z)));
    float yaw = glm::degrees(glm::atan(direction.x / direction.z));
    yaw = direction.z < 0.0f ? yaw - 180.0f : yaw;
    viewMatrix = glm::rotate(viewMatrix, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
    viewMatrix = glm::rotate(viewMatrix, glm::radians(yaw * -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    center.z -= zOffset;
    viewMatrix = glm::translate(viewMatrix, center * -1.0f);

    return viewMatrix;
}
