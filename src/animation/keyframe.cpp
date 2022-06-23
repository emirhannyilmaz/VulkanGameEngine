#include "keyframe.hpp"

Keyframe::Keyframe(float timestamp, std::unordered_map<std::string, JointTransform> pose) {
    this->timestamp = timestamp;
    this->pose = pose;
}
