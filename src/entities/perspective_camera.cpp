#include "perspective_camera.hpp"

PerspectiveCamera::PerspectiveCamera(glm::vec3 position, glm::vec3 rotation, float fieldOfView, float aspectRatio, float nearPlane, float farPlane) {
    this->position = position;
    this->rotation = rotation;
    this->fieldOfView = fieldOfView;
    this->aspectRatio = aspectRatio;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;

    nearPlaneWidth = nearPlane * glm::tan(glm::radians(fieldOfView));
    farPlaneWidth = SHADOW_DISTANCE * glm::tan(glm::radians(fieldOfView));
}

void PerspectiveCamera::update(float speed, float deltaTime) {
    nearPlaneHeight = nearPlaneWidth / aspectRatio;
    farPlaneHeight = farPlaneWidth / aspectRatio;

    rotation.x += (float) Input::GetMouseDy();
    if (rotation.x >= 90.0f) {
        rotation.x = 90.0f;
    }
    if (rotation.x <= -90.0f) {
        rotation.x = -90.0f;
    }
    rotation.y += (float) Input::GetMouseDx();

    glm::mat4 viewMatrix = createViewMatrix();
    glm::vec4 forwardCalculation = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f) * viewMatrix;
    forward = glm::normalize(glm::vec3(forwardCalculation.x, forwardCalculation.y, forwardCalculation.z));
    glm::vec4 upCalculation = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f) * viewMatrix;
    up = glm::normalize(glm::vec3(upCalculation.x, upCalculation.y, upCalculation.z));
    down = up * -1.0f;
    right = glm::cross(forward, up);
    left = right * -1.0f;

    if (Input::GetKey(GLFW_KEY_W)) {
        position += forward * speed * deltaTime;
    }

    if (Input::GetKey(GLFW_KEY_S)) {
        position -= forward * speed * deltaTime;
    }

    if (Input::GetKey(GLFW_KEY_A)) {
        position -= right * speed * deltaTime;
    }

    if (Input::GetKey(GLFW_KEY_D)) {
        position += right * speed * deltaTime;
    }
}

glm::mat4 PerspectiveCamera::createViewMatrix() {
    glm::mat4 matrix(1.0f);
    matrix = glm::rotate(matrix, glm::radians(rotation.x * -1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(rotation.y * -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(rotation.z * -1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    matrix = glm::translate(matrix, position * -1.0f);

    return matrix;
}

glm::mat4 PerspectiveCamera::createProjectionMatrix() {
    return glm::perspectiveLH_ZO(glm::radians(fieldOfView), aspectRatio, nearPlane, farPlane);
}

void PerspectiveCamera::invert(float distance) {
    oldPosition = position;
    oldRotation = rotation;

    position.y += distance;
    rotation.x = -rotation.x;
}

void PerspectiveCamera::revert() {
    position = oldPosition;
    rotation = oldRotation;
}

std::array<glm::vec3, 8> PerspectiveCamera::createFrustumVertices() {
    glm::vec3 toNearPlane = forward * nearPlane;
    glm::vec3 toFarPlane = forward * SHADOW_DISTANCE;
    glm::vec3 centerNearPlane = toNearPlane + position;
    glm::vec3 centerFarPlane = toFarPlane + position;

    glm::vec3 nearTop = centerNearPlane + up * nearPlaneHeight;
    glm::vec3 nearBottom = centerNearPlane + down * nearPlaneHeight;
    glm::vec3 farTop = centerFarPlane + up * farPlaneHeight;
    glm::vec3 farBottom = centerFarPlane + down * farPlaneHeight;

    std::array<glm::vec3, 8> vertices;
    vertices[0] = nearTop + right * nearPlaneWidth;
    vertices[1] = nearTop + left * nearPlaneWidth;
    vertices[2] = nearBottom + right * nearPlaneWidth;
    vertices[3] = nearBottom + left * nearPlaneWidth;
    vertices[4] = farTop + right * farPlaneWidth;
    vertices[5] = farTop + left * farPlaneWidth;
    vertices[6] = farBottom + right * farPlaneWidth;
    vertices[7] = farBottom + left * farPlaneWidth;

    return vertices;
}
