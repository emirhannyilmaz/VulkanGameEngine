#ifndef particle_hpp
#define particle_hpp

#include <glm/glm.hpp>
#include "../rendering/renderer/common.hpp"

class Particle {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::vec3 velocity;
    float gravityMultiplier;
    float lifeLength;
    float elapsedTime = 0.0f;
    Particle(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec3 velocity, float gravityMultiplier, float lifeLength);
    bool update(float deltaTime);
};

#endif
