#include "input.hpp"

GLFWwindow* Input::window = nullptr;
float Input::sensitivity = 0.0f;
std::unordered_map<int, bool> Input::keyStates = {};

bool Input::GetKey(int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

bool Input::GetKeyDown(int key) {
    std::cout << keyStates.size() << std::endl;

    if (keyStates.find(key) == keyStates.end()) {
        keyStates[key] = false;
    }

    if (glfwGetKey(window, key) == GLFW_PRESS) {
        if (keyStates[key] == false) {
            keyStates[key] = true;
            return true;
        } else {
            return false;
        }
    } else {
        keyStates[key] = false;
        return false;
    }
}

double Input::GetMouseDx() {
    static bool firstRun = true;
    static double previousPosition;
    double position;
    glfwGetCursorPos(window, &position, nullptr);

    if (firstRun) {
        previousPosition = position;
        firstRun = false;
    }

    double d = position - previousPosition;
    previousPosition = position;
    return d * (double) sensitivity;
}

double Input::GetMouseDy() {
    static bool firstRun = true;
    static double previousPosition;
    double position;
    glfwGetCursorPos(window, nullptr, &position);

    if (firstRun) {
        previousPosition = position;
        firstRun = false;
    }

    double d = position - previousPosition;
    previousPosition = position;
    return d * (double) sensitivity * -1.0f;
}
