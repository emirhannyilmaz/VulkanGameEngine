#ifndef semaphore_hpp
#define semaphore_hpp

#include <vulkan/vulkan.h>
#include <stdexcept>

class Semaphore {
public:
    VkSemaphore semaphore;
    Semaphore(VkDevice device);
    ~Semaphore();
private:
    VkDevice device;
};

#endif
