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
    rotation.x += (float) Input::getMouseDy() * deltaTime;
    rotation.y += (float) Input::getMouseDx() * deltaTime;

    glm::vec3 direction = glm::normalize(glm::vec3(glm::sin(glm::radians(rotation.y)), glm::sin(glm::radians(rotation.x)) * -1.0f, glm::cos(glm::radians(rotation.y))));
    glm::vec3 right = glm::cross(direction, glm::vec3(0.0f, -1.0f, 0.0f));

    if (Input::getKey(GLFW_KEY_W)) {
        position += direction * 5.0f * deltaTime;
    }

    if (Input::getKey(GLFW_KEY_S)) {
        position -= direction * 5.0f * deltaTime;
    }

    if (Input::getKey(GLFW_KEY_A)) {
        position -= right * 5.0f * deltaTime;
    }

    if (Input::getKey(GLFW_KEY_D)) {
        position += right * 5.0f * deltaTime;
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
    matrix[1][1] *= -1.0f;

    return matrix;
}
