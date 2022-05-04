#ifndef camera_hpp
#define camera_hpp

#include "../io/input.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    float fieldOfView;
    float aspectRatio;
    float nearPlane;
    float farPlane;
    float speed;
    Camera(glm::vec3 position, glm::vec3 rotation, float fieldOfView, float aspectRatio, float nearPlane, float farPlane, float speed);
    void update(float deltaTime);
    glm::mat4 createViewMatrix();
    glm::mat4 createProjectionMatrix();
    void invert(float distance);
    void revert();
private:
    glm::vec3 oldPosition;
    glm::vec3 oldRotation;
};

#endif
