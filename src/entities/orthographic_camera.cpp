#include "orthographic_camera.hpp"

void OrthographicCamera::update(PerspectiveCamera* perspectiveCamera, glm::mat4 transformationMatrix) {
    std::array<glm::vec3, 8> frustumVertices = perspectiveCamera->createFrustumVertices();

    bool first = true;
    for (glm::vec3 frustumVertex : frustumVertices) {
        glm::vec4 transformedFrustumVertex = glm::vec4(frustumVertex.x, frustumVertex.y, frustumVertex.z, 0.0f) * transformationMatrix;
        frustumVertex = glm::vec3(transformedFrustumVertex.x, transformedFrustumVertex.y, transformedFrustumVertex.z);

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
    glm::mat4 matrix(1.0f);
    matrix[0][0] = 2.0f / (maxX - minX);
    matrix[1][1] = 2.0f / (maxY - minY);
    matrix[2][2] = 1.0f / (maxZ - minZ);
    matrix[3][3] = 1.0f;

    return matrix;
}

float OrthographicCamera::getCenterOfZ() {
    return (maxZ - minZ) / 2.0f;
}
