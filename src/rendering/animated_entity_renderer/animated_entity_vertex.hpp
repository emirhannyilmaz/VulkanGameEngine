#ifndef animated_entity_vertex_hpp
#define animated_entity_vertex_hpp

#include <vulkan/vulkan.h>
#include <array>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

struct AnimatedEntityVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoordinates;
    glm::ivec3 jointIDs;
    glm::vec3 jointWeights;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(AnimatedEntityVertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 5> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 5> attributeDescriptions{};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(AnimatedEntityVertex, position);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(AnimatedEntityVertex, normal);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(AnimatedEntityVertex, textureCoordinates);

        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SINT;
        attributeDescriptions[3].offset = offsetof(AnimatedEntityVertex, jointIDs);

        attributeDescriptions[4].binding = 0;
        attributeDescriptions[4].location = 4;
        attributeDescriptions[4].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[4].offset = offsetof(AnimatedEntityVertex, jointWeights);

        return attributeDescriptions;
    }

    bool operator==(const AnimatedEntityVertex& other) const {
        return position == other.position && normal == other.normal && textureCoordinates == other.textureCoordinates;
    }
};

namespace std {
    template<> struct hash<AnimatedEntityVertex> {
        size_t operator()(AnimatedEntityVertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.position) ^ (hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.textureCoordinates) << 1);
        }
    };
}

#endif
