#include "renderer.hpp"
#include "../entities/skybox.hpp"

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
    renderPass = new RenderPass(device->device, swapchain->swapchainImageFormat, DepthResources::findDepthFormat(device->physicalDevice), device->msaaSamples);
    commandPool = new CommandPool(device->device, device->indices.graphicsFamily.value(), VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    colorResources = new ColorResources(device->physicalDevice, device->device, swapchain->swapchainExtent, device->msaaSamples, swapchain->swapchainImageFormat);
    depthResources = new DepthResources(device->physicalDevice, device->device, swapchain->swapchainExtent, device->msaaSamples, commandPool->commandPool, device->graphicsQueue);

    framebuffers.resize(swapchain->swapchainImageViews.size());
    for (size_t i = 0; i < framebuffers.size(); i++) {
        std::array<VkImageView, 3> attachments = {
            colorResources->image->imageView,
            depthResources->image->imageView,
            swapchain->swapchainImageViews[i]
        };
        framebuffers[i] = new Framebuffer(device->device, renderPass->renderPass, static_cast<uint32_t>(attachments.size()), attachments.data(), swapchain->swapchainExtent);
    }

    commandBuffers = new CommandBuffers(device->device, commandPool->commandPool, MAX_FRAMES_IN_FLIGHT);

    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        imageAvailableSemaphores[i] = new Semaphore(device->device);
        renderFinishedSemaphores[i] = new Semaphore(device->device);
        inFlightFences[i] = new Fence(device->device, VK_FENCE_CREATE_SIGNALED_BIT);
    }
}

Renderer::~Renderer() {
    cleanUpSwapchain();

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        delete imageAvailableSemaphores[i];
        delete renderFinishedSemaphores[i];
        delete inFlightFences[i];
    }
    imageAvailableSemaphores.clear();
    renderFinishedSemaphores.clear();
    inFlightFences.clear();

    delete commandPool;
    delete device;

    if (enableValidationLayers) {
        delete messenger;
    }

    delete surface;
    delete instance;
}

void Renderer::beginRendering() {
    vkWaitForFences(device->device, 1, &inFlightFences[currentFrame]->fence, VK_TRUE, UINT64_MAX);

    calculateDeltaTime();

    uint32_t imageIndex;
    VkResult acquireNextImageResult = vkAcquireNextImageKHR(device->device, swapchain->swapchain, UINT64_MAX, imageAvailableSemaphores[currentFrame]->semaphore, VK_NULL_HANDLE, &imageIndex);
    if (acquireNextImageResult == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapchain();
        return;
    } else if (acquireNextImageResult != VK_SUCCESS && acquireNextImageResult != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("Failed to acquire swap chain image!");
    }

    vkResetFences(device->device, 1, &inFlightFences[currentFrame]->fence);
    vkResetCommandBuffer(commandBuffers->commandBuffers[currentFrame], 0);

    VkCommandBufferBeginInfo commandBufferBeginInfo{};
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(commandBuffers->commandBuffers[currentFrame], &commandBufferBeginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassBeginInfo{};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass = renderPass->renderPass;
    renderPassBeginInfo.framebuffer = framebuffers[imageIndex]->framebuffer;
    renderPassBeginInfo.renderArea.offset = {0, 0};
    renderPassBeginInfo.renderArea.extent = swapchain->swapchainExtent;
    std::array<VkClearValue, 2> clearValues{};
    clearValues[0] = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clearValues[1] = {{1.0f, 0}};
    renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassBeginInfo.pClearValues = clearValues.data();
    vkCmdBeginRenderPass(commandBuffers->commandBuffers[currentFrame], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void Renderer::endRendering() {
    vkCmdEndRenderPass(commandBuffers->commandBuffers[currentFrame]);

    if (vkEndCommandBuffer(commandBuffers->commandBuffers[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer!");
    }

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &imageAvailableSemaphores[currentFrame]->semaphore;
    VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    submitInfo.pWaitDstStageMask = &waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers->commandBuffers[currentFrame];
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &renderFinishedSemaphores[currentFrame]->semaphore;

    if (vkQueueSubmit(device->graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]->fence) != VK_SUCCESS) {
        throw std::runtime_error("Failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderFinishedSemaphores[currentFrame]->semaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchain->swapchain;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    VkResult queuePresentResult = vkQueuePresentKHR(device->presentQueue, &presentInfo);
    if (queuePresentResult == VK_ERROR_OUT_OF_DATE_KHR || queuePresentResult == VK_SUBOPTIMAL_KHR || window->framebufferResized) {
        window->framebufferResized = false;
        recreateSwapchain();
    } else if (queuePresentResult != VK_SUCCESS) {
        throw std::runtime_error("Failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Renderer::calculateDeltaTime() {
    static auto renderStartTime = std::chrono::system_clock::now();
    auto renderFinishTime = std::chrono::system_clock::now();

    deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(renderFinishTime - renderStartTime).count();

    renderStartTime = std::chrono::system_clock::now();
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
    renderPass = new RenderPass(device->device, swapchain->swapchainImageFormat, DepthResources::findDepthFormat(device->physicalDevice), device->msaaSamples);
    depthResources = new DepthResources(device->physicalDevice, device->device, swapchain->swapchainExtent, device->msaaSamples, commandPool->commandPool, device->graphicsQueue);
    colorResources = new ColorResources(device->physicalDevice, device->device, swapchain->swapchainExtent, device->msaaSamples, swapchain->swapchainImageFormat);

    framebuffers.resize(swapchain->swapchainImageViews.size());
    for (size_t i = 0; i < framebuffers.size(); i++) {
        std::array<VkImageView, 3> attachments = {
            colorResources->image->imageView,
            depthResources->image->imageView,
            swapchain->swapchainImageViews[i]
        };
        framebuffers[i] = new Framebuffer(device->device, renderPass->renderPass, static_cast<uint32_t>(attachments.size()), attachments.data(), swapchain->swapchainExtent);
    }

    camera->aspectRatio = (float) swapchain->swapchainExtent.width / (float) swapchain->swapchainExtent.height;
}

void Renderer::cleanUpSwapchain() {
    for (auto framebuffer : framebuffers) {
        delete framebuffer;
    }
    framebuffers.clear();

    delete colorResources;
    delete depthResources;
    delete renderPass;
    delete swapchain;
}
