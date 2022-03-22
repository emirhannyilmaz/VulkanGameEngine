#ifndef window_hpp
#define window_hpp

#include <GLFW/glfw3.h>
#include <string>

class Window {
public:
    int width;
    int height;
    std::string title;
    GLFWwindow* window;
    bool framebufferResized = false;
    Window(int width, int height, std::string title);
    ~Window();
private:
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
};

#endif
