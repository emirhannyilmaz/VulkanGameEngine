#ifndef orthographic_camera_hpp
#define orthographic_camera_hpp

#include "perspective_camera.hpp"

class OrthographicCamera {
public:
    void update(PerspectiveCamera* perspectiveCamera, glm::mat4 transformationMatrix);
    glm::mat4 createProjectionMatrix();
    float getCenterOfZ();
private:
    float minX;
    float maxX;
    float minY;
    float maxY;
    float minZ;
    float maxZ;
};

#endif
