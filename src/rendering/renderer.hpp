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

class Renderer {
public:
    Instance* instance;
    Messenger* messenger;
    Surface* surface;
    Device* device;
    Swapchain* swapchain;
    RenderPass* renderPass;
    CommandPool* commandPool;
    ColorResources* colorResources;
    DepthResources* depthResources;
    std::vector<Framebuffer*> framebuffers;
    CommandBuffers* commandBuffers;
    std::vector<Semaphore*> imageAvailableSemaphores;
    std::vector<Semaphore*> renderFinishedSemaphores;
    std::vector<Fence*> inFlightFences;
    uint32_t currentFrame = 0;
    float deltaTime = 0.0f;
    Renderer(Window* window, Camera* camera);
    ~Renderer();
    void beginRendering();
    void endRendering();
private:
    Window* window;
    Camera* camera;
    void calculateDeltaTime();
    void recreateSwapchain();
    void cleanUpSwapchain();
};

#endif
