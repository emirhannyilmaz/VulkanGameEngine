#ifndef particle_system_hpp
#define particle_system_hpp

#include "../rendering/renderer/renderer.hpp"
#include "../entities/particle.hpp"

class ParticleSystem {
public:
    ParticleSystem(std::string textureAtlasFileName, int textureAtlasRowCount, float rotation, float scale, int particlesPerSecond, float speed, float gravityMultiplier, float lifeLength, Renderer* renderer);
    void emit(glm::vec3 position, std::vector<Particle*>& particles);
private:
    std::string textureAtlasFileName;
    int textureAtlasRowCount;
    float rotation;
    float scale;
    int particlesPerSecond;
    float speed;
    float gravityMultiplier;
    float lifeLength;
    Renderer* renderer;
    void createParticle(glm::vec3 position, std::vector<Particle*>& particles);
};

#endif
