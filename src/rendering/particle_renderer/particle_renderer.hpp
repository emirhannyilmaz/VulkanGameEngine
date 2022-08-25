#ifndef particle_renderer_hpp
#define particle_renderer_hpp

#include "../renderer/renderer.hpp"
#include "../../entities/particle.hpp"
#include "../renderer/graphics_pipeline.hpp"
#include "../renderer/descriptor_set_layout.hpp"
#include "../renderer/descriptor_pool.hpp"
#include "../renderer/descriptor_sets.hpp"
#include "../renderer/buffer.hpp"

struct ParticleRendererVertexUniformBufferObject {
    alignas(16) glm::mat4 projectionMatrix;
};

struct ParticleRendererFragmentUniformBufferObject {
    alignas(16) glm::vec3 fogColor;
};

struct ParticleRendererVertexPushConstants {
    alignas(16) glm::mat4 viewMatrix;
    alignas(16) glm::vec4 clipPlane;
};

class ParticleRenderer {
public:
    ParticleRenderer(Renderer* renderer);
    ~ParticleRenderer();
    void CreateGraphicsPipelines();
    void DeleteGraphicsPipelines();
    void render(std::vector<Particle*> particles, PerspectiveCamera* perspectiveCamera, glm::vec4 clipPlane, CommandBuffers* commandBuffers, bool onScreen);
private:
    Renderer* renderer;
    DescriptorSetLayout* descriptorSetLayout;
    GraphicsPipeline* graphicsPipeline;
    GraphicsPipeline* offScreenGraphicsPipeline;
    DescriptorPool* descriptorPool;
    DescriptorSets* descriptorSets;
    std::vector<Buffer*> vertexUniformBuffers;
    std::vector<Buffer*> fragmentUniformBuffers;
    ParticleRendererVertexPushConstants vertexPushConstants{};
    void updateDescriptorSetResources(PerspectiveCamera* perspectiveCamera);
    void updatePushConstants(PerspectiveCamera* perspectiveCamera, glm::vec4 clipPlane);
};

#endif
