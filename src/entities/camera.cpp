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
    if (rotation.x >= 90.0f) {
        rotation.x = 90.0f;
    }
    if (rotation.x <= -90.0f) {
        rotation.x = -90.0f;
    }

    rotation.y += (float) Input::GetMouseDx() * deltaTime;
    if (rotation.y >= 360.0f) {
        rotation.y = 0.0f;
    }

    glm::vec3 direction = glm::normalize(glm::vec3(glm::sin(glm::radians(rotation.y)) * glm::cos(glm::radians(rotation.x)), glm::sin(glm::radians(rotation.x)) * -1.0f, glm::cos(glm::radians(rotation.y)) * glm::cos(glm::radians(rotation.x))));

    float upX = glm::sin(glm::radians(rotation.y));
    if ((rotation.y >= 90.0f && rotation.y <= 180.0f) || (rotation.y >= 270.0f && rotation.y <= 0.0f)) {
        upX = glm::cos(glm::radians(rotation.y));
    }
    upX *= glm::sin(glm::radians(rotation.x));

    float upZ = glm::cos(glm::radians(rotation.y));
    if ((rotation.y >= 90.0f && rotation.y <= 180.0f) || (rotation.y >= 270.0f && rotation.y <= 0.0f)) {
        upZ = glm::sin(glm::radians(rotation.y));
    }
    upZ *= glm::sin(glm::radians(rotation.x));

    glm::vec3 up = glm::vec3(upX, glm::sin(glm::radians(rotation.x)) * glm::cos(glm::radians(rotation.x)) * -1.0f, upZ);
    glm::vec3 right = glm::cross(direction, up);

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
