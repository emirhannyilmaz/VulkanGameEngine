#ifndef joint_transform_hpp
#define joint_transform_hpp

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct JointTransform {
    glm::vec3 position;
    glm::quat rotation;

    glm::mat4 createTransformationMatrix() {
        glm::mat4 matrix(1.0f);
        matrix = glm::translate(matrix, position);
        matrix *= glm::mat4_cast(rotation);
        return matrix;
    }

    static JointTransform Interpolate(JointTransform x, JointTransform y, float a) {
        return JointTransform{glm::mix(x.position, y.position, a), glm::slerp(x.rotation, y.rotation, a)};
    }
};

#endif
