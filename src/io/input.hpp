#ifndef input_hpp
#define input_hpp

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <iostream>

class Input {
public:
    static GLFWwindow* window;
    static float sensitivity;
    static bool GetKey(int key);
    static bool GetKeyDown(int key);
    static double GetMouseDx();
    static double GetMouseDy();
private:
    static std::unordered_map<int, bool> keyStates;
};

#endif
