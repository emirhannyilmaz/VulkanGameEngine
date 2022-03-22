#include "input.hpp"

GLFWwindow* Input::window = nullptr;
float Input::sensitivity = 0.0f;

bool Input::getKey(int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

double Input::getMouseDx() {
    static double previousPosition;
    double position;
    glfwGetCursorPos(window, &position, nullptr);

    double d = position - previousPosition;
    previousPosition = position;
    return d * (double) sensitivity;
}

double Input::getMouseDy() {
    static double previousPosition;
    double position;
    glfwGetCursorPos(window, nullptr, &position);

    double d = position - previousPosition;
    previousPosition = position;
    return d * (double) sensitivity * -1.0f;
}
