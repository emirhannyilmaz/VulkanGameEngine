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

struct ParticleVertexUniformBufferObject {
    alignas(16) glm::mat4 modelViewMatrix;
};

class Particle {
public:
    static DescriptorSetLayout* descriptorSetLayout;
    DescriptorSets* descriptorSets;
    glm::vec3 position;
    float rotation;
    glm::vec3 scale;
    glm::vec3 velocity;
    float gravityMultiplier;
    float lifeLength;
    float elapsedTime = 0.0f;
    Particle(glm::vec3 position, float rotation, glm::vec3 scale, glm::vec3 velocity, float gravityMultiplier, float lifeLength, Renderer* renderer);
    ~Particle();
    bool update(float deltaTime);
    void updateDescriptorSetResources(glm::mat4 viewMatrix);
    static void CreateDesriptorSetLayout(VkDevice& device);
    static void DeleteDesriptorSetLayout();
private:
    DescriptorPool* descriptorPool;
    Renderer* renderer;
    std::vector<Buffer*> vertexUniformBuffers;
};

#endif
