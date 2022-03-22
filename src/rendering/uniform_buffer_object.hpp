#ifndef uniform_buffer_object_hpp
#define uniform_buffer_object_hpp

#include <glm/glm.hpp>

struct UniformBufferObject {
    alignas(16) glm::mat4 modelMatrix;
    alignas(16) glm::mat4 viewMatrix;
    alignas(16) glm::mat4 projectionMatrix;
};

#endif
