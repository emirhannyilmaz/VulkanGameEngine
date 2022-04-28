#ifndef renderer_hpp
#define renderer_hpp

#include "instance.hpp"
#include "messenger.hpp"
#include "surface.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "render_pass.hpp"
#include "command_pool.hpp"
#include "color_resources.hpp"
#include "depth_resources.hpp"
#include "framebuffer.hpp"
#include "command_buffers.hpp"
#include "semaphore.hpp"
#include "fence.hpp"
#include "sampler.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include "../io/window.hpp"
#include "../entities/camera.hpp"
#include "water_resources.hpp"
#include <array>

class Renderer {
public:
    Instance* instance;
    Messenger* messenger = nullptr;
    Surface* surface;
    Device* device;
    Swapchain* swapchain;
    RenderPass* renderPass;
    CommandPool* commandPool;
    ColorResources* colorResources;
    DepthResources* depthResources;
    std::vector<Framebuffer*> framebuffers;
    WaterResources* waterResources;
    CommandBuffers* commandBuffers;
    CommandBuffers* offScreenCommandBuffers;
    std::vector<Semaphore*> imageAvailableSemaphores;
    std::vector<Semaphore*> renderFinishedSemaphores;
    std::vector<Semaphore*> offScreenRenderFinishedSemaphores;
    std::vector<Fence*> inFlightFences;
    float deltaTime = 0.0f;
    uint32_t currentFrame = 0;
    uint32_t currentImageIndex;
    Renderer(Window* window, Camera* camera);
    ~Renderer();
    void beginDrawing();
    void endDrawing();
    void beginRendering(RenderPass* renderPass, Framebuffer* framebuffer, CommandBuffers* commandBuffers);
    void endRendering(CommandBuffers* commandBuffers);
private:
    Window* window;
    Camera* camera;
    VkResult acquireNextImageResult;
    void calculateDeltaTime();
    void recreateSwapchain();
    void cleanUpSwapchain();
};

#endif
