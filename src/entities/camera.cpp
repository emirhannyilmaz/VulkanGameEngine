#include "camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 rotation, float fieldOfView, float aspectRatio, float nearPlane, float farPlane, float speed) {
    this->position = position;
    this->rotation = rotation;
    this->fieldOfView = fieldOfView;
    this->aspectRatio = aspectRatio;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;
    this->speed = speed;
}

void Camera::update(float deltaTime) {
    rotation.x += (float) Input::GetMouseDy() * deltaTime;
    if (rotation.x >= 89.9f) {
        rotation.x = 89.9f;
    }
    if (rotation.x <= -89.9f) {
        rotation.x = -89.9f;
    }

    rotation.y += (float) Input::GetMouseDx() * deltaTime;

    glm::vec3 direction = glm::normalize(glm::vec3(glm::sin(glm::radians(rotation.y)) * glm::cos(glm::radians(rotation.x)), glm::sin(glm::radians(rotation.x)) * -1.0f, glm::cos(glm::radians(rotation.y)) * glm::cos(glm::radians(rotation.x))));
    glm::vec3 up = glm::vec3(0.0f, -1.0f, 0.0f);
    glm::vec3 right = glm::normalize(glm::cross(direction, up));

    if (Input::GetKey(GLFW_KEY_W)) {
        position += direction * speed * deltaTime;
    }

    if (Input::GetKey(GLFW_KEY_S)) {
        position -= direction * speed * deltaTime;
    }

    if (Input::GetKey(GLFW_KEY_A)) {
        position -= right * speed * deltaTime;
    }

    if (Input::GetKey(GLFW_KEY_D)) {
        position += right * speed * deltaTime;
    }
}

glm::mat4 Camera::createViewMatrix() {
    glm::mat4 matrix(1.0f);
    matrix = glm::rotate(matrix, glm::radians(rotation.x * -1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(rotation.y * -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(rotation.z * -1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    matrix = glm::translate(matrix, position * -1.0f);

    return matrix;
}

glm::mat4 Camera::createProjectionMatrix() {
    glm::mat4 matrix = glm::perspectiveLH_ZO(glm::radians(fieldOfView), aspectRatio, nearPlane, farPlane);

    return matrix;
}

void Camera::invert(float distance) {
    oldPosition = position;
    oldRotation = rotation;

    position.y += distance;
    rotation.x = -rotation.x;
}

void Camera::revert() {
    position = oldPosition;
    rotation = oldRotation;
}
