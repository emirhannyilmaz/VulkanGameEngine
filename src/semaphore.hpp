#ifndef semaphore_hpp
#define semaphore_hpp

#define VK_ENABLE_BETA_EXTENSIONS
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Semaphore {
public:
    VkSemaphore semaphore;
    Semaphore(VkDevice device);
    ~Semaphore();
private:
    VkDevice device;
};

#endif
