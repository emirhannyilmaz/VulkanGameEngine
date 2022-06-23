#ifndef joint_transform_hpp
#define joint_transform_hpp

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class JointTransform {
public:
    glm::vec3 position;
    glm::quat rotation;
    JointTransform(glm::vec3 position, glm::quat rotation);
    glm::mat4 createTransformationMatrix();
    static JointTransform Interpolate(JointTransform& x, JointTransform& y, float a);
};

#endif
