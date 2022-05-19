#ifndef perspective_camera_hpp
#define perspective_camera_hpp

#include "../io/input.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

class PerspectiveCamera {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    float fieldOfView;
    float aspectRatio;
    float nearPlane;
    float farPlane;
    float nearPlaneWidth;
    float nearPlaneHeight;
    float farPlaneWidth;
    float farPlaneHeight;
    float speed;
    glm::vec3 forward;
    glm::vec3 up;
    glm::vec3 down;
    glm::vec3 right;
    glm::vec3 left;
    PerspectiveCamera(glm::vec3 position, glm::vec3 rotation, float fieldOfView, float aspectRatio, float nearPlane, float farPlane);
    void update(float speed, float deltaTime);
    glm::mat4 createViewMatrix();
    glm::mat4 createProjectionMatrix();
    void invert(float distance);
    void revert();
    std::array<glm::vec3, 8> createFrustumVertices();
private:
    glm::vec3 oldPosition;
    glm::vec3 oldRotation;
};

#endif
