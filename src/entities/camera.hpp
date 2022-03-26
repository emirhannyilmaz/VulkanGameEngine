#ifndef camera_hpp
#define camera_hpp

#include "../io/input.hpp"
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    float fieldOfView;
    float aspectRatio;
    float nearPlane;
    float farPlane;
    Camera(glm::vec3 position, glm::vec3 rotation, float fieldOfView, float aspectRatio, float nearPlane, float farPlane);
    void update(float deltaTime);
    glm::mat4 createViewMatrix();
    glm::mat4 createProjectionMatrix();
};

#endif
