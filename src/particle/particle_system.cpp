#include "particle_system.hpp"

ParticleSystem::ParticleSystem(std::string textureFileName, int textureRowCount, int particlesPerSecond, float speed, float gravityMultiplier, float lifeLength, Renderer* renderer) {
    this->textureFileName = textureFileName;
    this->textureRowCount = textureRowCount;
    this->particlesPerSecond = particlesPerSecond;
    this->speed = speed;
    this->gravityMultiplier = gravityMultiplier;
    this->lifeLength = lifeLength;
    this->renderer = renderer;
}

void ParticleSystem::emit(glm::vec3 position, std::vector<Particle*>& particles) {
    float particlesPerFrame = (float) particlesPerSecond * renderer->realDeltaTime;
    int particlesPerFrameInt = (int) std::floor(particlesPerFrame);
    float partialParticle = fmod(particlesPerFrame, 1.0f);
    for (int i = 0; i < particlesPerFrameInt; i++) {
        createParticle(position, particles);
    }
    if ((float) rand() / (float) RAND_MAX < partialParticle) {
        createParticle(position, particles);
    }
}

void ParticleSystem::createParticle(glm::vec3 position, std::vector<Particle*>& particles) {
    glm::vec3 velocity = glm::vec3(((float) rand() / (float) RAND_MAX) * 2.0f - 1.0f, -1.0f, ((float) rand() / (float) RAND_MAX) * 2.0f - 1.0f);
    velocity = glm::normalize(velocity);
    velocity *= speed;
    Texture* texture = new Texture(textureFileName, textureRowCount, renderer);
    particles.push_back(new Particle(texture, position, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f), velocity, gravityMultiplier, lifeLength, renderer));
}
