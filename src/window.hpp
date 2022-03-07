#ifndef window_hpp
#define window_hpp

#include <GLFW/glfw3.h>
#include <string>
#include "app_info.hpp"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class Window {
public:
    GLFWwindow* window;
    bool framebufferResized = false;
    Window();
    ~Window();
private:
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
};

#endif
