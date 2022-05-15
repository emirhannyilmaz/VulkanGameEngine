#ifndef light_hpp
#define light_hpp

#include <glm/glm.hpp>
#include "../io/input.hpp"

class Light {
public:
    glm::vec3 position;
    glm::vec3 color;
    Light(glm::vec3 position, glm::vec3 color);
    void update(float speed, float deltaTime);
};

#endif
