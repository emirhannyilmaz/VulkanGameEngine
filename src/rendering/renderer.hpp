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
#include "../entities/perspective_camera.hpp"
#include "water_resources.hpp"
#include "shadow_map_resources.hpp"
#include <array>
#include "query_pool.hpp"

class EntityRenderer;
class ShadowMapRenderer;
class TerrainRenderer;
class SkyboxRenderer;
class WaterRenderer;

class Renderer {
public:
    Instance* instance;
    Messenger* messenger = nullptr;
    Surface* surface;
    Device* device;
    Swapchain* swapchain;
    RenderPass* renderPass;
    CommandPool* commandPool;
    QueryPool* queryPool;
    QueryPool* offScreenQueryPool;
    ColorResources* colorResources;
    DepthResources* depthResources;
    std::vector<Framebuffer*> framebuffers;
    WaterResources* waterResources;
    ShadowMapResources* shadowMapResources;
    CommandBuffers* commandBuffers;
    CommandBuffers* offScreenCommandBuffers;
    std::vector<Semaphore*> imageAvailableSemaphores;
    std::vector<Semaphore*> renderFinishedSemaphores;
    std::vector<Semaphore*> offScreenRenderFinishedSemaphores;
    std::vector<Fence*> inFlightFences;
    VkExtent2D shadowMapExtent{SHADOW_MAP_SIZE, SHADOW_MAP_SIZE};
    double deltaTime = 0.0f;
    uint32_t currentFrame = 0;
    uint32_t currentImageIndex;
    TerrainRenderer* terrainRenderer;
    EntityRenderer* entityRenderer;
    ShadowMapRenderer* shadowMapRenderer;
    SkyboxRenderer* skyboxRenderer;
    WaterRenderer* waterRenderer;
    Renderer(Window* window, PerspectiveCamera* perspectiveCamera);
    ~Renderer();
    void waitIdle();
    void beginDrawing();
    void endDrawing();
    void beginRecordingCommands(CommandBuffers* commandBuffers, bool onScreen);
    void endRecordingCommands(CommandBuffers* commandBuffers, bool onScreen);
    void beginRendering(RenderPass* renderPass, Framebuffer* framebuffer, CommandBuffers* commandBuffers, bool hasColorAttachment);
    void endRendering(CommandBuffers* commandBuffers);
private:
    Window* window;
    PerspectiveCamera* perspectiveCamera;
    VkResult acquireNextImageResult;
    void recreateSwapchain();
    void cleanUpSwapchain();
};

#endif
