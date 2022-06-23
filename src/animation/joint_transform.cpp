#include "joint_transform.hpp"

JointTransform::JointTransform(glm::vec3 position, glm::quat rotation) {
    this->position = position;
    this->rotation = rotation;
}

glm::mat4 JointTransform::createTransformationMatrix() {
    glm::mat4 matrix(1.0f);
    matrix = glm::translate(matrix, position);
    matrix *= glm::mat4_cast(rotation);
    return matrix;
}

JointTransform JointTransform::Interpolate(JointTransform& x, JointTransform& y, float a) {
    return JointTransform(glm::mix(x.position, y.position, a), glm::slerp(x.rotation, y.rotation, a));
}
