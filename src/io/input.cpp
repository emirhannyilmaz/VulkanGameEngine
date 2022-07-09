#include "input.hpp"

GLFWwindow* Input::window = nullptr;
float Input::sensitivity = 0.0f;

bool Input::GetKey(int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
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
