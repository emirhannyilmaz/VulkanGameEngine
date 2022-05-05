#include "renderer.hpp"
#include "entity_renderer.hpp"
#include "skybox_renderer.hpp"
#include "water_renderer.hpp"

Renderer::Renderer(Window* window, Camera* camera) {
    this->window = window;
    this->camera = camera;

    instance = new Instance(window->title);

    if (enableValidationLayers) {
        messenger = new Messenger(instance->instance);
    }

    surface = new Surface(instance->instance, window->window);
    device = new Device(instance->instance, surface->surface);
    swapchain = new Swapchain(device->device, surface->surface, device->swapchainSupportDetails, window->window, device->indices);
    renderPass = new RenderPass(device->device, swapchain->swapchainImageFormat, DepthResources::findDepthFormat(device->physicalDevice), device->msaaSamples, true);
    commandPool = new CommandPool(device->device, device->indices.graphicsFamily.value(), VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    queryPool = new QueryPool(device->device, 2 * MAX_FRAMES_IN_FLIGHT);
    offScreenQueryPool = new QueryPool(device->device, 2 * MAX_FRAMES_IN_FLIGHT);
    colorResources = new ColorResources(device->physicalDevice, device->device, swapchain->swapchainExtent, device->msaaSamples, swapchain->swapchainImageFormat, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
    depthResources = new DepthResources(device->physicalDevice, device->device, swapchain->swapchainExtent, device->msaaSamples, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);

    framebuffers.resize(swapchain->swapchainImageViews.size());
    for (size_t i = 0; i < framebuffers.size(); i++) {
        std::array<VkImageView, 3> attachments = {
            colorResources->image->imageView,
            depthResources->image->imageView,
            swapchain->swapchainImageViews[i]
        };
        framebuffers[i] = new Framebuffer(device->device, renderPass->renderPass, static_cast<uint32_t>(attachments.size()), attachments.data(), swapchain->swapchainExtent);
    }

    waterResources = new WaterResources(device->physicalDevice, device->device, swapchain->swapchainExtent, swapchain->swapchainImageFormat, commandPool->commandPool, device->graphicsQueue);
    commandBuffers = new CommandBuffers(device->device, commandPool->commandPool, MAX_FRAMES_IN_FLIGHT);
    offScreenCommandBuffers = new CommandBuffers(device->device, commandPool->commandPool, MAX_FRAMES_IN_FLIGHT);

    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    offScreenRenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        imageAvailableSemaphores[i] = new Semaphore(device->device);
        renderFinishedSemaphores[i] = new Semaphore(device->device);
        offScreenRenderFinishedSemaphores[i] = new Semaphore(device->device);
        inFlightFences[i] = new Fence(device->device, VK_FENCE_CREATE_SIGNALED_BIT);
    }
}

Renderer::~Renderer() {
    cleanUpSwapchain();

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        delete inFlightFences[i];
        delete offScreenRenderFinishedSemaphores[i];
        delete renderFinishedSemaphores[i];
        delete imageAvailableSemaphores[i];
    }
    inFlightFences.clear();
    offScreenRenderFinishedSemaphores.clear();
    renderFinishedSemaphores.clear();
    imageAvailableSemaphores.clear();

    delete offScreenQueryPool;
    delete queryPool;
    delete commandPool;
    delete device;
    delete surface;
    delete messenger;
    delete instance;
}

void Renderer::waitIdle() {
    vkDeviceWaitIdle(device->device);
}

void Renderer::beginDrawing() {
    vkWaitForFences(device->device, 1, &inFlightFences[currentFrame]->fence, VK_TRUE, UINT64_MAX);

    acquireNextImageResult = vkAcquireNextImageKHR(device->device, swapchain->swapchain, UINT64_MAX, imageAvailableSemaphores[currentFrame]->semaphore, VK_NULL_HANDLE, &currentImageIndex);
    if (acquireNextImageResult == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapchain();
        return;
    } else if (acquireNextImageResult != VK_SUCCESS && acquireNextImageResult != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("Failed to acquire swap chain image!");
    }

    vkResetFences(device->device, 1, &inFlightFences[currentFrame]->fence);
    vkResetCommandBuffer(commandBuffers->commandBuffers[currentFrame], 0);
    vkResetCommandBuffer(offScreenCommandBuffers->commandBuffers[currentFrame], 0);
}

void Renderer::endDrawing() {
    if (acquireNextImageResult == VK_ERROR_OUT_OF_DATE_KHR) {
        return;
    }

    std::array<VkSubmitInfo, 2> submitInfos{};
    submitInfos[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfos[0].waitSemaphoreCount = 0;
    submitInfos[0].pWaitSemaphores = nullptr;
    submitInfos[0].pWaitDstStageMask = nullptr;
    submitInfos[0].commandBufferCount = 1;
    submitInfos[0].pCommandBuffers = &offScreenCommandBuffers->commandBuffers[currentFrame];
    submitInfos[0].signalSemaphoreCount = 1;
    submitInfos[0].pSignalSemaphores = &offScreenRenderFinishedSemaphores[currentFrame]->semaphore;

    submitInfos[1].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    std::array<VkSemaphore, 2> waitSemaphores = {
        offScreenRenderFinishedSemaphores[currentFrame]->semaphore,
        imageAvailableSemaphores[currentFrame]->semaphore
    };
    std::array<VkPipelineStageFlags, 2> waitStages = {
        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };
    submitInfos[1].waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
    submitInfos[1].pWaitSemaphores = waitSemaphores.data();
    submitInfos[1].pWaitDstStageMask = waitStages.data();
    submitInfos[1].commandBufferCount = 1;
    submitInfos[1].pCommandBuffers = &commandBuffers->commandBuffers[currentFrame];
    submitInfos[1].signalSemaphoreCount = 1;
    submitInfos[1].pSignalSemaphores = &renderFinishedSemaphores[currentFrame]->semaphore;

    if (vkQueueSubmit(device->graphicsQueue, static_cast<uint32_t>(submitInfos.size()), submitInfos.data(), inFlightFences[currentFrame]->fence) != VK_SUCCESS) {
        throw std::runtime_error("Failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderFinishedSemaphores[currentFrame]->semaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchain->swapchain;
    presentInfo.pImageIndices = &currentImageIndex;
    presentInfo.pResults = nullptr;

    VkResult queuePresentResult = vkQueuePresentKHR(device->presentQueue, &presentInfo);
    if (queuePresentResult == VK_ERROR_OUT_OF_DATE_KHR || queuePresentResult == VK_SUBOPTIMAL_KHR || window->framebufferResized) {
        window->framebufferResized = false;
        recreateSwapchain();
    } else if (queuePresentResult != VK_SUCCESS) {
        throw std::runtime_error("Failed to present swap chain image!");
    }

    std::array<uint64_t, 2> offScreenTimestamps{};
    vkGetQueryPoolResults(device->device, offScreenQueryPool->queryPool, currentFrame * 2, 2, sizeof(offScreenTimestamps), offScreenTimestamps.data(), sizeof(uint64_t), VK_QUERY_RESULT_WAIT_BIT | VK_QUERY_RESULT_64_BIT);
    double offScreenDeltaTime = double(offScreenTimestamps[1] - offScreenTimestamps[0]) * device->timestampPeriod;

    std::array<uint64_t, 2> onScreenTimestamps{};
    vkGetQueryPoolResults(device->device, queryPool->queryPool, currentFrame * 2, 2, sizeof(onScreenTimestamps), onScreenTimestamps.data(), sizeof(uint64_t), VK_QUERY_RESULT_WAIT_BIT | VK_QUERY_RESULT_64_BIT);
    double onScreenDeltaTime = double(onScreenTimestamps[1] - onScreenTimestamps[0]) * device->timestampPeriod;

    deltaTime = (offScreenDeltaTime + onScreenDeltaTime) / 1000000000.0f;

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Renderer::beginRecordingCommands(CommandBuffers* commandBuffers, bool onScreen) {
    if (acquireNextImageResult == VK_ERROR_OUT_OF_DATE_KHR) {
        return;
    }

    VkCommandBufferBeginInfo commandBufferBeginInfo{};
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(commandBuffers->commandBuffers[currentFrame], &commandBufferBeginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }

    vkCmdResetQueryPool(commandBuffers->commandBuffers[currentFrame], onScreen ? queryPool->queryPool : offScreenQueryPool->queryPool, currentFrame * 2, 2);
    vkCmdWriteTimestamp(commandBuffers->commandBuffers[currentFrame], VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, onScreen ? queryPool->queryPool : offScreenQueryPool->queryPool, currentFrame * 2);
}

void Renderer::endRecordingCommands(CommandBuffers* commandBuffers, bool onScreen) {
    if (acquireNextImageResult == VK_ERROR_OUT_OF_DATE_KHR) {
        return;
    }

    vkCmdWriteTimestamp(commandBuffers->commandBuffers[currentFrame], VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, onScreen ? queryPool->queryPool : offScreenQueryPool->queryPool, currentFrame * 2 + 1);

    if (vkEndCommandBuffer(commandBuffers->commandBuffers[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer!");
    }
}

void Renderer::beginRendering(RenderPass* renderPass, Framebuffer* framebuffer, CommandBuffers* commandBuffers) {
    if (acquireNextImageResult == VK_ERROR_OUT_OF_DATE_KHR) {
        return;
    }

    VkRenderPassBeginInfo renderPassBeginInfo{};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass = renderPass->renderPass;
    renderPassBeginInfo.framebuffer = framebuffer->framebuffer;
    renderPassBeginInfo.renderArea.offset = {0, 0};
    renderPassBeginInfo.renderArea.extent = swapchain->swapchainExtent;
    std::array<VkClearValue, 2> clearValues{};
    clearValues[0] = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clearValues[1] = {{1.0f, 0}};
    renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassBeginInfo.pClearValues = clearValues.data();
    vkCmdBeginRenderPass(commandBuffers->commandBuffers[currentFrame], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void Renderer::endRendering(CommandBuffers* commandBuffers) {
    if (acquireNextImageResult == VK_ERROR_OUT_OF_DATE_KHR) {
        return;
    }

    vkCmdEndRenderPass(commandBuffers->commandBuffers[currentFrame]);
}

void Renderer::recreateSwapchain() {
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(window->window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window->window, &width, &height);
        glfwWaitEvents();
    }
    vkDeviceWaitIdle(device->device);

    cleanUpSwapchain();

    swapchain = new Swapchain(device->device, surface->surface, device->swapchainSupportDetails, window->window, device->indices);
    renderPass = new RenderPass(device->device, swapchain->swapchainImageFormat, DepthResources::findDepthFormat(device->physicalDevice), device->msaaSamples, true);
    colorResources = new ColorResources(device->physicalDevice, device->device, swapchain->swapchainExtent, device->msaaSamples, swapchain->swapchainImageFormat, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
    depthResources = new DepthResources(device->physicalDevice, device->device, swapchain->swapchainExtent, device->msaaSamples, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);

    framebuffers.resize(swapchain->swapchainImageViews.size());
    for (size_t i = 0; i < framebuffers.size(); i++) {
        std::array<VkImageView, 3> attachments = {
            colorResources->image->imageView,
            depthResources->image->imageView,
            swapchain->swapchainImageViews[i]
        };
       framebuffers[i] = new Framebuffer(device->device, renderPass->renderPass, static_cast<uint32_t>(attachments.size()), attachments.data(), swapchain->swapchainExtent);
    }

    waterResources = new WaterResources(device->physicalDevice, device->device, swapchain->swapchainExtent, swapchain->swapchainImageFormat, commandPool->commandPool, device->graphicsQueue);
    waterRenderer->updateDescriptorSetImageInfos();
    waterRenderer->CreateGraphicsPipeline();
    skyboxRenderer->CreateGraphicsPipelines();
    entityRenderer->CreateGraphicsPipelines();

    camera->aspectRatio = (float) swapchain->swapchainExtent.width / (float) swapchain->swapchainExtent.height;
}

void Renderer::cleanUpSwapchain() {
    entityRenderer->DeleteGraphicsPipelines();
    skyboxRenderer->DeleteGraphicsPipelines();
    waterRenderer->DeleteGraphicsPipeline();
    delete waterResources;

    for (auto framebuffer : framebuffers) {
        delete framebuffer;
    }
    framebuffers.clear();

    delete depthResources;
    delete colorResources;
    delete renderPass;
    delete swapchain;
}
