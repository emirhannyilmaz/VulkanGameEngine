#ifndef particle_hpp
#define particle_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../rendering/renderer/renderer.hpp"
#include "../rendering/renderer/buffer.hpp"
#include "../rendering/renderer/descriptor_pool.hpp"
#include "../rendering/renderer/descriptor_set_layout.hpp"
#include "../rendering/renderer/descriptor_sets.hpp"
#include "../rendering/renderer/common.hpp"
#include "../rendering/renderer/texture.hpp"

struct ParticleVertexUniformBufferObject {
    alignas(16) glm::mat4 modelMatrix;
};

struct ParticleVertexPushConstants {
    alignas(16) glm::mat4 modelViewMatrix;
};

class Particle {
public:
    static DescriptorSetLayout* descriptorSetLayout;
    DescriptorSets* descriptorSets;
    ParticleVertexPushConstants vertexPushConstants{};
    Texture* texture;
    glm::vec3 position;
    float rotation;
    glm::vec3 scale;
    glm::vec3 velocity;
    float gravityMultiplier;
    float lifeLength;
    float elapsedTime = 0.0f;
    std::optional<uint32_t> deleteAtFrame;
    Particle(Texture* texture, glm::vec3 position, float rotation, glm::vec3 scale, glm::vec3 velocity, float gravityMultiplier, float lifeLength, Renderer* renderer);
    ~Particle();
    bool update(float deltaTime, float realDeltaTime);
    void updateDescriptorSetResources();
    void updatePushConstants(PerspectiveCamera* perspectiveCamera);
    static void CreateDesriptorSetLayout(VkDevice& device);
    static void DeleteDesriptorSetLayout();
private:
    DescriptorPool* descriptorPool;
    Renderer* renderer;
    std::vector<Buffer*> vertexUniformBuffers;
};

#endif
