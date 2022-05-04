#ifndef input_hpp
#define input_hpp

#include <GLFW/glfw3.h>

class Input {
public:
    static GLFWwindow* window;
    static float sensitivity;
    static bool GetKey(int key);
    static double GetMouseDx();
    static double GetMouseDy();
};

#endif
