#ifndef post_processing_manager_hpp
#define post_processing_manager_hpp

#include "io/input.hpp"

class PostProcessingManager {
public:
    bool enableGaussianBlur = false;
    void update();
};

#endif
