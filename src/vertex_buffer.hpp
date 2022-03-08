#ifndef vertex_buffer_hpp
#define vertex_buffer_hpp

#include <vulkan/vulkan.h>
#include <vector>
#include "vertex.hpp"
#include <stdexcept>

class VertexBuffer {
public:
    VkBuffer vertexBuffer;
    VertexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, std::vector<Vertex> vertices);
    ~VertexBuffer();
private:
    VkDevice device;
    VkDeviceMemory vertexBufferMemory;
    uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
};

#endif
