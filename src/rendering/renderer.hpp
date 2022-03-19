#ifndef renderer_hpp
#define renderer_hpp

#include "renderer_info.hpp"
#include "instance.hpp"
#include "messenger.hpp"
#include "surface.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "render_pass.hpp"
#include "graphics_pipeline.hpp"
#include "framebuffer.hpp"
#include "command_pool.hpp"
#include "command_buffers.hpp"
#include "semaphore.hpp"
#include "fence.hpp"
#include "vertex.hpp"
#include "buffer.hpp"
#include "descriptor_set_layout.hpp"
#include "descriptor_pool.hpp"
#include "descriptor_sets.hpp"
#include "uniform_buffer_object.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include "../io/window.hpp"
#include "../entities/entity.hpp"

class Renderer {
public:
    Instance* instance;
    Messenger* messenger;
    Surface* surface;
    Device* device;
    Swapchain* swapchain;
    RenderPass* renderPass;
    DescriptorSetLayout* descriptorSetLayout;
    GraphicsPipeline* graphicsPipeline;
    std::vector<Framebuffer*> framebuffers;
    CommandPool* commandPool;
    std::vector<Buffer*> uniformBuffers;
    DescriptorPool* descriptorPool;
    DescriptorSets* descriptorSets;
    CommandBuffers* commandBuffers;
    std::vector<Semaphore*> imageAvailableSemaphores;
    std::vector<Semaphore*> renderFinishedSemaphores;
    std::vector<Fence*> inFlightFences;
    Renderer(Window* window);
    ~Renderer();
    void render(Entity* entity);
private:
    Window* window;
    void recreateSwapchain();
    void cleanUpSwapchain();
    void updateUniformBuffer(Entity* entity, uint32_t currentFrame);
    void recordCommandBuffer(uint32_t currentFrame, uint32_t imageIndex, Mesh* mesh);
};

#endif
