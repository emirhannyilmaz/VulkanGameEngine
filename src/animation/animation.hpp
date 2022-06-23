#ifndef animation_hpp
#define animation_hpp

#include <vector>
#include "keyframe.hpp"

class Animation {
public:
    float length;
    std::vector<Keyframe> keyframes;
    Animation(float length, std::vector<Keyframe> keyframes);
};

#endif
