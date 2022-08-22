#ifndef particle_hpp
#define particle_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../rendering/renderer/common.hpp"

struct ParticleVertexPushConstants {
    alignas(16) glm::mat4 modelViewMatrix;
};

class Particle {
public:
    glm::vec3 position;
    float rotation;
    glm::vec3 scale;
    glm::vec3 velocity;
    float gravityMultiplier;
    float lifeLength;
    float elapsedTime = 0.0f;
    ParticleVertexPushConstants vertexPushConstants{};
    Particle(glm::vec3 position, float rotation, glm::vec3 scale, glm::vec3 velocity, float gravityMultiplier, float lifeLength);
    bool update(float deltaTime);
    void updatePushConstants(glm::mat4 viewMatrix);
};

#endif
