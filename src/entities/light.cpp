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
}
