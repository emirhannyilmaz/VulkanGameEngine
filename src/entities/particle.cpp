#include "particle.hpp"

Particle::Particle(glm::vec3 position, float rotation, glm::vec3 scale, glm::vec3 velocity, float gravityMultiplier, float lifeLength) {
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
    this->velocity = velocity;
    this->gravityMultiplier = gravityMultiplier;
    this->lifeLength = lifeLength;
}

bool Particle::update(float deltaTime) {
    velocity.y += GRAVITY * gravityMultiplier * deltaTime;
    position += velocity * deltaTime;
    elapsedTime += deltaTime;
    return elapsedTime < lifeLength;
}

void Particle::updatePushConstants(glm::mat4 viewMatrix) {
    glm::mat4 modelMatrix(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix[0][0] = viewMatrix[0][0];
    modelMatrix[0][1] = viewMatrix[1][0];
    modelMatrix[0][2] = viewMatrix[2][0];
    modelMatrix[1][0] = viewMatrix[0][1];
    modelMatrix[1][1] = viewMatrix[1][1];
    modelMatrix[1][2] = viewMatrix[2][1];
    modelMatrix[2][0] = viewMatrix[0][2];
    modelMatrix[2][1] = viewMatrix[1][2];
    modelMatrix[2][2] = viewMatrix[2][2];
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, scale);

    vertexPushConstants.modelViewMatrix = modelMatrix * viewMatrix;
}
