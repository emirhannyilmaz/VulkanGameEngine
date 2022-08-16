#include "particle.hpp"

Particle::Particle(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec3 velocity, float gravityMultiplier, float lifeLength) {
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
