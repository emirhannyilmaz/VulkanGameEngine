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
#include "../rendering/renderer/texture_atlas.hpp"

struct ParticleVertexUniformBufferObject {
    alignas(16) glm::mat4 modelMatrix;
    alignas(8) glm::vec2 textureOffset1;
    alignas(8) glm::vec2 textureOffset2;
    alignas(4) int rowCount;
};

struct ParticleFragmentUniformBufferObject {
    alignas(4) float blendFactor;
};

struct ParticleVertexPushConstants {
    alignas(16) glm::mat4 modelViewMatrix;
};

class Particle {
public:
    static DescriptorSetLayout* descriptorSetLayout;
    DescriptorSets* descriptorSets;
    ParticleVertexPushConstants vertexPushConstants{};
    TextureAtlas* textureAtlas;
    glm::vec3 position;
    float rotation;
    glm::vec3 scale;
    glm::vec3 velocity;
    float gravityMultiplier;
    float lifeLength;
    float elapsedTime = 0.0f;
    glm::vec2 textureOffset1;
    glm::vec2 textureOffset2;
    float blendFactor;
    float distanceFromCamera;
    std::optional<uint32_t> deleteAtFrame;
    Particle(TextureAtlas* textureAtlas, glm::vec3 position, float rotation, glm::vec3 scale, glm::vec3 velocity, float gravityMultiplier, float lifeLength, Renderer* renderer);
    ~Particle();
    bool update(PerspectiveCamera* perspectiveCamera, float deltaTime, float realDeltaTime);
    void updateDescriptorSetResources();
    void updatePushConstants(PerspectiveCamera* perspectiveCamera);
    static void CreateDesriptorSetLayout(VkDevice& device);
    static void DeleteDesriptorSetLayout();
private:
    DescriptorPool* descriptorPool;
    Renderer* renderer;
    std::vector<Buffer*> vertexUniformBuffers;
    std::vector<Buffer*> fragmentUniformBuffers;
    void updateTextureAtlasInfo();
};

#endif
