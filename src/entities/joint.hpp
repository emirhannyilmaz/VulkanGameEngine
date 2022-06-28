#ifndef joint_hpp
#define joint_hpp

#include <vector>
#include <string>
#include <glm/glm.hpp>

struct Joint {
    int index;
    std::string name;
    glm::mat4 localBindTransform;
    std::vector<Joint> children;
    glm::mat4 animatedTransform = glm::mat4(1.0f);
    glm::mat4 inverseBindTransform = glm::mat4(1.0f);

    void calculateInverseBindTransform(glm::mat4 parentBindTransform) {
        glm::mat4 bindTransform = parentBindTransform * localBindTransform;
        inverseBindTransform = glm::inverse(bindTransform);
        for (Joint child : children) {
            child.calculateInverseBindTransform(bindTransform);
        }
    }
};

#endif
