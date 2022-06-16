#ifndef joint_hpp
#define joint_hpp

#include <vector>
#include <string>
#include <glm/glm.hpp>

class Joint {
public:
    int index;
    std::string name;
    std::vector<Joint> children;
    glm::mat4 animatedTransform(1.0f);
    glm::mat4 inverseBindTransform(1.0f);
    Joint(int index, std::string name, glm::mat4 localBindTransform);
    void calculateInverseBindTransform(glm::mat4 parentBindTransform);
private:
    glm::mat4 localBindTransform;
};

#endif
