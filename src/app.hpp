#ifndef app_hpp
#define app_hpp

#include "io/window.hpp"
#include "rendering/renderer.hpp"
#include "rendering/model_loader.hpp"

class App {
public:
    void run();
private:
    Window* window;
    Camera* camera;
    Light* light;
    Renderer* renderer;
};

#endif
