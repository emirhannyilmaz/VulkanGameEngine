#ifndef light_hpp
#define light_hpp

#include <glm/glm.hpp>

class Light {
public:
    glm::vec3 position;
    glm::vec3 color;
    Light(glm::vec3 position, glm::vec3 color);
};

#endif
