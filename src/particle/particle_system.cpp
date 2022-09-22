#include "particle_system.hpp"

ParticleSystem::ParticleSystem(std::string textureAtlasFileName, int textureAtlasRowCount, int particlesPerSecond, float speed, float gravityMultiplier, float lifeLength, Renderer* renderer) {
    this->textureAtlasFileName = textureAtlasFileName;
    this->textureAtlasRowCount = textureAtlasRowCount;
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
    glm::vec3 velocity = glm::vec3(0.0f, -1.0f, 0.0f);
    velocity = glm::normalize(velocity);
    velocity *= speed;
    TextureAtlas* textureAtlas = new TextureAtlas(textureAtlasFileName, textureAtlasRowCount, renderer);
    particles.push_back(new Particle(textureAtlas, position, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f), velocity, gravityMultiplier, lifeLength, renderer));
}
