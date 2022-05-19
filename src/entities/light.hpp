#ifndef light_hpp
#define light_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../io/input.hpp"

class Light {
public:
    glm::vec3 position;
    glm::vec3 color;
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    Light(glm::vec3 position, glm::vec3 color);
    void update(float speed, float deltaTime);
    glm::mat4 createViewMatrix(glm::vec3 center);
};

#endif
