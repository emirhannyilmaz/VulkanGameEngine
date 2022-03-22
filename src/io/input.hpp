#ifndef input_hpp
#define input_hpp

#include <GLFW/glfw3.h>

class Input {
public:
    static GLFWwindow* window;
    static float sensitivity;
    static bool getKey(int key);
    static double getMouseDx();
    static double getMouseDy();
};

#endif
