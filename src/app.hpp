#ifndef app_hpp
#define app_hpp

#include "io/window.hpp"
#include "rendering/renderer.hpp"
#include "rendering/vertex.hpp"

const std::vector<Vertex> vertices = {
    {
        {-0.5f, -0.5f},
        {1.0f, 0.0f, 0.0f}
    },
    {
        {0.5f, -0.5f},
        {0.0f, 1.0f, 0.0f}
    },
    {
        {0.5f, 0.5f},
        {0.0f, 0.0f, 1.0f}
    },
    {
        {-0.5f, 0.5f},
        {1.0f, 1.0f, 1.0f}
    }
};

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
};

class App {
public:
    void run();
private:
    Window* window;
    Camera* camera;
    Renderer* renderer;
};

#endif
