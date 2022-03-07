#include "window.hpp"

Window::Window() {
    glfwInit();
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    
    window = glfwCreateWindow(WIDTH, HEIGHT, TITLE.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto windowClass = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    windowClass->framebufferResized = true;
}
