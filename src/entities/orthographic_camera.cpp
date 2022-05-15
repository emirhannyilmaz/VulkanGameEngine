#include "orthographic_camera.hpp"

void OrthographicCamera::update(PerspectiveCamera* perspectiveCamera) {
    std::array<glm::vec3, 8> frustumVertices = perspectiveCamera->getFrustumVertices();

    bool first = true;
    for (glm::vec3 frustumVertex : frustumVertices) {
        if (first) {
            minX = frustumVertex.x;
            maxX = frustumVertex.x;
            minY = frustumVertex.y;
            maxY = frustumVertex.y;
            minZ = frustumVertex.z;
            maxZ = frustumVertex.z;
            first = false;
            continue;
        }

        if (frustumVertex.x > maxX) {
            maxX = frustumVertex.x;
        } else if (frustumVertex.x < minX) {
            minX = frustumVertex.x;
        }

        if (frustumVertex.y > maxY) {
            maxY = frustumVertex.y;
        } else if (frustumVertex.y < minY) {
            minY = frustumVertex.y;
        }

        if (frustumVertex.z > maxZ) {
            maxZ = frustumVertex.z;
        } else if (frustumVertex.z < minZ) {
            minZ = frustumVertex.z;
        }
    }
}

glm::mat4 OrthographicCamera::createProjectionMatrix() {
    return glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
}

glm::vec3 OrthographicCamera::getCenter() {
    return glm::vec3((minX + maxX) / 2.0f, (minY + maxY) / 2.0f, (minZ + maxZ) / 2.0f);
}
