#include "surface.hpp"

Surface::Surface(VkInstance instance, GLFWwindow* window) {
    this->instance = instance;

    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface!");
    }
}

Surface::~Surface() {
    vkDestroySurfaceKHR(instance, surface, nullptr);
}
