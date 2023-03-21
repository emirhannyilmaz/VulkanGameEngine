#include "post_processing_manager.hpp"

void PostProcessingManager::update() {
    if (Input::GetKeyDown(GLFW_KEY_G)) {
        enableGaussianBlur = !enableGaussianBlur;
    }
}
