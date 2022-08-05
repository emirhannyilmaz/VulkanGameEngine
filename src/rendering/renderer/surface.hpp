#ifndef surface_hpp
#define surface_hpp

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

class Surface {
public:
    VkSurfaceKHR surface;
    Surface(VkInstance& instance, GLFWwindow* window);
    ~Surface();
private:
    VkInstance instance;
};

#endif
