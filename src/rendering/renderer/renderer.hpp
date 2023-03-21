#ifndef renderer_hpp
#define renderer_hpp

#include "instance.hpp"
#include "messenger.hpp"
#include "surface.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "default_render_pass.hpp"
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
#include "../../io/window.hpp"
#include "../../entities/perspective_camera.hpp"
#include "../water_renderer/water_resources.hpp"
#include "../shadow_map_renderer/shadow_map_resources.hpp"
#include "../post_processing/gaussian_blur_resources.hpp"
#include <array>
#include "query_pool.hpp"

class EntityRenderer;
class AnimatedEntityRenderer;
class EntityShadowMapRenderer;
class AnimatedEntityShadowMapRenderer;
class TerrainRenderer;
class SkyboxRenderer;
class WaterRenderer;
class ParticleRenderer;
class HorizontalGaussianBlurPostProcessing;
class VerticalGaussianBlurPostProcessing;

class Renderer {
public:
    Instance* instance;
    Messenger* messenger = nullptr;
    Surface* surface;
    Device* device;
    Swapchain* swapchain;
    DefaultRenderPass* renderPass;
    CommandPool* commandPool;
    QueryPool* queryPool;
    ColorResources* colorResources;
    DepthResources* depthResources;
    std::vector<Framebuffer*> framebuffers;
    WaterResources* waterResources;
    ShadowMapResources* shadowMapResources;
    GaussianBlurResources* gaussianBlurResources;
    CommandBuffers* commandBuffers;
    CommandBuffers* offScreenCommandBuffers;
    std::vector<Semaphore*> imageAvailableSemaphores;
    std::vector<Semaphore*> renderFinishedSemaphores;
    std::vector<Semaphore*> offScreenRenderFinishedSemaphores;
    std::vector<Fence*> inFlightFences;
    VkExtent2D shadowMapExtent{SHADOW_MAP_SIZE, SHADOW_MAP_SIZE};
    std::array<bool, 2> isFirstTimeFrameRender {true, true};
    std::vector<float> deltaTimes{};
    float deltaTime = 0.0f;
    float realDeltaTime = 0.0f;
    float previousTime = -1.0f;
    uint32_t currentFrame = 0;
    uint32_t currentImageIndex;
    TerrainRenderer* terrainRenderer;
    EntityRenderer* entityRenderer;
    AnimatedEntityRenderer* animatedEntityRenderer;
    EntityShadowMapRenderer* entityShadowMapRenderer;
    AnimatedEntityShadowMapRenderer* animatedEntityShadowMapRenderer;
    SkyboxRenderer* skyboxRenderer;
    WaterRenderer* waterRenderer;
    ParticleRenderer* particleRenderer;
    HorizontalGaussianBlurPostProcessing* horizontalGaussianBlurPostProcessing;
    VerticalGaussianBlurPostProcessing* verticalGaussianBlurPostProcessing;
    Renderer(Window* window, PerspectiveCamera* perspectiveCamera);
    ~Renderer();
    void waitIdle();
    void beginDrawing();
    void endDrawing();
    void beginRecordingCommands(CommandBuffers* commandBuffers);
    void endRecordingCommands(CommandBuffers* commandBuffers);
    void beginRendering(RenderPass* renderPasss, Framebuffer* framebufferr, CommandBuffers* commandBuffers, bool hasColorAttachment, bool hasDepthAttachment);
    void endRendering(CommandBuffers* commandBuffers);
private:
    Window* window;
    PerspectiveCamera* perspectiveCamera;
    VkResult acquireNextImageResult;
    void recreateSwapchain();
    void cleanUpSwapchain();
};

#endif
