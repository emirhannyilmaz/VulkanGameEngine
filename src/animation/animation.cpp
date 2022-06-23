#include "animation.hpp"

Animation::Animation(float length, std::vector<Keyframe> keyframes) {
    this->length = length;
    this->keyframes = keyframes;
}
