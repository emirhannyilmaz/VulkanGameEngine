#ifndef orthographic_camera_hpp
#define orthographic_camera_hpp

#include "perspective_camera.hpp"

class OrthographicCamera {
public:
    float minX;
    float maxX;
    float minY;
    float maxY;
    float minZ;
    float maxZ;
    void update(PerspectiveCamera* perspectiveCamera, glm::mat4 transformationMatrix);
    glm::mat4 createProjectionMatrix();
    glm::vec3 getCenter();
};

#endif
