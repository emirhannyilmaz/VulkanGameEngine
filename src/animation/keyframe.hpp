#ifndef keyframe_hpp
#define keyframe_hpp

#include <unordered_map>
#include <string>
#include "joint_transform.hpp"

class Keyframe {
public:
    float timestamp;
    std::unordered_map<std::string, JointTransform> pose;
    Keyframe(float timestamp, std::unordered_map<std::string, JointTransform> pose);
};

#endif
