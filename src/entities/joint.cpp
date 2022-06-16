#include "joint.hpp"

Joint::Joint(int index, std::string name, glm::mat4 localBindTransform) {
    this->index = index;
    this->name = name;
    this->localBindTransform = localBindTransform;
}

void Joint::calculateInverseBindTransform(glm::mat4 parentBindTransform) {
    glm::mat4 bindTransform = parentBindTransform * localBindTransform;
    inverseBindTransform = glm::inverse(bindTransform);
    for (Joint child : children) {
        child.calculateInverseBindTransform(bindTransform);
    }
}
