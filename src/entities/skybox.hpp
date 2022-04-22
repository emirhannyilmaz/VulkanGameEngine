#ifndef skybox_hpp
#define skybox_hpp

#include "../rendering/texture.hpp"
#include "../rendering/descriptor_pool.hpp"
#include "../rendering/descriptor_set_layout.hpp"
#include "../rendering/descriptor_sets.hpp"
#include "../rendering/renderer_info.hpp"
#include "../rendering/buffer.hpp"

struct SkyboxVertexUniformBufferObject {
    alignas(4) float size;
};

class Skybox {
public:
    Texture* texture;
    float size;
    static DescriptorSetLayout* descriptorSetLayout;
    DescriptorSets* descriptorSets;
    Skybox(Texture* texture, float size, Renderer* renderer);
    ~Skybox();
    static void CreateDesriptorSetLayout(VkDevice& device);
    static void DeleteDesriptorSetLayout();
private:
    DescriptorPool* descriptorPool;
    Renderer* renderer;
    std::vector<Buffer*> vertexUniformBuffers;
};

#endif
