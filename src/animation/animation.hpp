#ifndef animation_hpp
#define animation_hpp

#include <vector>
#include "keyframe.hpp"

struct Animation {
    float length;
    std::vector<Keyframe> keyframes;
};

#endif
