#include "camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 rotation, float fieldOfView, float aspectRatio, float nearPlane, float farPlane) {
    this->position = position;
    this->rotation = rotation;
    this->fieldOfView = fieldOfView;
    this->aspectRatio = aspectRatio;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;
}

void Camera::update(float deltaTime) {
    if (Input::getKey(GLFW_KEY_W)) {
        position.z += 5.0f * deltaTime;
    }

    if (Input::getKey(GLFW_KEY_S)) {
        position.z -= 5.0f * deltaTime;
    }

    if (Input::getKey(GLFW_KEY_A)) {
        position.x -= 5.0f * deltaTime;
    }

    if (Input::getKey(GLFW_KEY_D)) {
        position.x += 5.0f * deltaTime;
    }

    rotation.x += (float) Input::getMouseDy() * deltaTime;
    rotation.y += (float) Input::getMouseDx() * deltaTime;
}

glm::mat4 Camera::createViewMatrix() {
    glm::mat4 matrix(1.0f);
    matrix = glm::translate(matrix, position * -1.0f);
    if (rotation.x != 0.0f) {
        matrix = glm::rotate(matrix, rotation.x * -1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    }

    if (rotation.y != 0.0f) {
        matrix = glm::rotate(matrix, rotation.y * -1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    if (rotation.z != 0.0f) {
        matrix = glm::rotate(matrix, rotation.z * -1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    }

    return matrix;
}

glm::mat4 Camera::createProjectionMatrix() {
    return glm::perspectiveLH_ZO(glm::radians(fieldOfView), aspectRatio, nearPlane, farPlane);
}
