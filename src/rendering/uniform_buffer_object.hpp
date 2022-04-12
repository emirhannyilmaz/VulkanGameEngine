#ifndef uniform_buffer_object_hpp
#define uniform_buffer_object_hpp

#include <glm/glm.hpp>

struct UniformBufferObject {
    alignas(16) glm::mat4 modelMatrix;
    alignas(16) glm::mat4 viewMatrix;
    alignas(16) glm::mat4 projectionMatrix;
    alignas(16) glm::vec3 lightPosition;
    alignas(16) glm::vec3 lightColor;
    alignas(4) float reflectivity;
    alignas(4) float shineDamper;
};

#endif
