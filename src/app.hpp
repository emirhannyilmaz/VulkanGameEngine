#ifndef app_hpp
#define app_hpp

#include "window.hpp"
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
#include "app_info.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

const std::vector<Vertex> vertices = {
    {
        {-0.5f, -0.5f},
        {1.0f, 0.0f, 0.0f}
    },
    {
        {0.5f, -0.5f},
        {0.0f, 1.0f, 0.0f}
    },
    {
        {0.5f, 0.5f},
        {0.0f, 0.0f, 1.0f}
    },
    {
        {-0.5f, 0.5f},
        {1.0f, 1.0f, 1.0f}
    }
};

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
};

class App {
public:
    void run();
private:
    Window* window;
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
    Buffer* vertexBuffer;
    Buffer* indexBuffer;
    std::vector<Buffer*> uniformBuffers;
    DescriptorPool* descriptorPool;
    DescriptorSets* descriptorSets;
    CommandBuffers* commandBuffers;
    std::vector<Semaphore*> imageAvailableSemaphores;
    std::vector<Semaphore*> renderFinishedSemaphores;
    std::vector<Fence*> inFlightFences;
    uint32_t currentFrame = 0;
    void initWindow();
    void initVulkan();
    void mainLoop();
    void drawFrame();
    void recreateSwapchain();
    void updateUniformBuffer(uint32_t currentImage);
    void cleanUpSwapchain();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void cleanUp();
};

#endif
