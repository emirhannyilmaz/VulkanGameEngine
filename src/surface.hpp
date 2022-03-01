#ifndef surface_hpp
#define surface_hpp

#define VK_ENABLE_BETA_EXTENSIONS
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Surface {
public:
    VkSurfaceKHR surface;
    Surface(VkInstance instance, GLFWwindow* window);
    ~Surface();
private:
    VkInstance instance;
};

#endif
