#include "orthographic_camera.hpp"

void OrthographicCamera::update(PerspectiveCamera* perspectiveCamera, glm::mat4 transformationMatrix) {
    std::array<glm::vec3, 8> frustumVertices = perspectiveCamera->createFrustumVertices();

    bool first = true;
    for (glm::vec3 frustumVertex : frustumVertices) {
        glm::vec4 frustumVertexVec4 = glm::vec4(frustumVertex.x, frustumVertex.y, frustumVertex.z, 1.0f) * glm::inverse(transformationMatrix);
        frustumVertex = glm::vec3(frustumVertexVec4.x, frustumVertexVec4.y, frustumVertexVec4.z);

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

        if (frustumVertex.y < maxY) {
            maxY = frustumVertex.y;
        } else if (frustumVertex.y > minY) {
            minY = frustumVertex.y;
        }

        if (frustumVertex.z > maxZ) {
            maxZ = frustumVertex.z;
        } else if (frustumVertex.z < minZ) {
            minZ = frustumVertex.z;
        }
    }

    maxZ += 10.0f;
}

glm::mat4 OrthographicCamera::createProjectionMatrix() {
    return glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
}

glm::vec3 OrthographicCamera::getCenter() {
    return glm::vec3((minX + maxX) / 2.0f, (minY + maxY) / 2.0f, (minZ + maxZ) / 2.0f);
}
