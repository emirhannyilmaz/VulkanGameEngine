#ifndef skybox_hpp
#define skybox_hpp

#include "../rendering/texture.hpp"
#include "../rendering/descriptor_pool.hpp"
#include "../rendering/descriptor_set_layout.hpp"
#include "../rendering/descriptor_sets.hpp"
#include "../rendering/renderer_info.hpp"
#include "camera.hpp"

struct SkyboxVertexUniformBufferObject {
    alignas(16) glm::mat4 viewMatrix;
    alignas(16) glm::mat4 projectionMatrix;
    alignas(4) float size;
};

class Skybox {
public:
    Texture* texture;
    float size;
    static DescriptorSetLayout* descriptorSetLayout;
    DescriptorSets* descriptorSets;
    Skybox(Texture* texture, float size, Camera* camera, Renderer* renderer);
    ~Skybox();
    void updateDescriptorSetResources(uint32_t currentFrame);
    static void CreateDesriptorSetLayout(VkDevice& device);
    static void DeleteDesriptorSetLayout();
private:
    DescriptorPool* descriptorPool;
    Camera* camera;
    Renderer* renderer;
    SkyboxVertexUniformBufferObject ubo{};
    std::vector<Buffer*> vertexUniformBuffers;
};

#endif
