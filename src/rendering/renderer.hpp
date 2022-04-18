#ifndef renderer_hpp
#define renderer_hpp

#include "renderer_info.hpp"
#include "instance.hpp"
#include "messenger.hpp"
#include "surface.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "render_pass.hpp"
#include "descriptor_set_layout.hpp"
#include "graphics_pipeline.hpp"
#include "command_pool.hpp"
#include "color_resources.hpp"
#include "depth_resources.hpp"
#include "framebuffer.hpp"
#include "command_buffers.hpp"
#include "semaphore.hpp"
#include "fence.hpp"
#include "vertex.hpp"
#include "buffer.hpp"
#include "descriptor_pool.hpp"
#include "descriptor_sets.hpp"
#include "sampler.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include "../io/window.hpp"
#include "../entities/entity.hpp"
#include "../entities/camera.hpp"
#include "../entities/light.hpp"
#include "texture.hpp"

struct GeneralVertexUniformBufferObject {
    alignas(16) glm::mat4 viewMatrix;
    alignas(16) glm::mat4 projectionMatrix;
    alignas(16) glm::vec3 lightPosition;
};

struct GeneralFragmentUniformBufferObject {
    alignas(16) glm::vec3 lightColor;
};

class Renderer {
public:
    DescriptorSetLayout* edescriptorSetLayout;
    Instance* instance;
    Messenger* messenger;
    Surface* surface;
    Device* device;
    Swapchain* swapchain;
    RenderPass* renderPass;
    DescriptorSetLayout* descriptorSetLayout;
    GraphicsPipeline* graphicsPipeline;
    CommandPool* commandPool;
    ColorResources* colorResources;
    DepthResources* depthResources;
    std::vector<Framebuffer*> framebuffers;
    std::vector<Buffer*> vertexUniformBuffers;
    std::vector<Buffer*> fragmentUniformBuffers;
    DescriptorPool* descriptorPool;
    DescriptorSets* descriptorSets;
    CommandBuffers* commandBuffers;
    std::vector<Semaphore*> imageAvailableSemaphores;
    std::vector<Semaphore*> renderFinishedSemaphores;
    std::vector<Fence*> inFlightFences;
    float deltaTime = 0.0f;
    Renderer(Window* window, Camera* camera);
    ~Renderer();
    void render(std::vector<Entity*> entities, Light* light);
private:
    Window* window;
    Camera* camera;
    GeneralVertexUniformBufferObject vertexUbo{};
    void calculateDeltaTime();
    void recreateSwapchain();
    void cleanUpSwapchain();
    void recordCommandBuffer(uint32_t currentFrame, uint32_t imageIndex, std::vector<Entity*> entities, Light* light);
};

#endif
