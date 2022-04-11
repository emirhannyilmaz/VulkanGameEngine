#include "renderer.hpp"

Renderer::Renderer(Window* window, Camera* camera) {
    this->window = window;
    this->camera = camera;

    ubo.projectionMatrix = camera->createProjectionMatrix();

    instance = new Instance(window->title);

    if (enableValidationLayers) {
        messenger = new Messenger(instance->instance);
    }

    surface = new Surface(instance->instance, window->window);
    device = new Device(instance->instance, surface->surface);
    swapchain = new Swapchain(device->device, surface->surface, device->swapchainSupportDetails, window->window, device->indices);
    renderPass = new RenderPass(device->device, swapchain->swapchainImageFormat, DepthResources::findDepthFormat(device->physicalDevice), device->msaaSamples);
    descriptorSetLayout = new DescriptorSetLayout(device->device);
    graphicsPipeline = new GraphicsPipeline(device->device, descriptorSetLayout->descriptorSetLayout, swapchain->swapchainExtent, renderPass->renderPass, device->msaaSamples);
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

    // Uniform buffers
    VkDeviceSize uniformBufferSize = sizeof(UniformBufferObject);
    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        uniformBuffers[i] = new Buffer(device->physicalDevice, device->device, uniformBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    }

    descriptorPool = new DescriptorPool(device->device);

    std::vector<VkBuffer> ub;
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        ub.push_back(uniformBuffers[i]->buffer);
    }
    descriptorSets = new DescriptorSets(device->device, descriptorPool->descriptorPool, descriptorSetLayout->descriptorSetLayout, ub);

    commandBuffers = new CommandBuffers(device->device, commandPool->commandPool, MAX_FRAMES_IN_FLIGHT);

    sampler = new Sampler(device->physicalDevice, device->device);

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
        delete uniformBuffers[i];
    }
    uniformBuffers.clear();

    delete sampler;

    delete descriptorPool;
    delete descriptorSetLayout;

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

void Renderer::render(Entity* entity) {
    static uint32_t currentFrame = 0;

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

    ubo.viewMatrix = camera->createViewMatrix();

    updateUniformBuffer(entity, currentFrame);
    descriptorSets->updateImageInfo(currentFrame, entity->texture->image->imageView, sampler->sampler);

    vkResetFences(device->device, 1, &inFlightFences[currentFrame]->fence);

    vkResetCommandBuffer(commandBuffers->commandBuffers[currentFrame], 0);
    recordCommandBuffer(currentFrame, imageIndex, entity->mesh);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {
        imageAvailableSemaphores[currentFrame]->semaphore
    };

    VkPipelineStageFlags waitStages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };

    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers->commandBuffers[currentFrame];

    VkSemaphore signalSemaphores[] = {
        renderFinishedSemaphores[currentFrame]->semaphore
    };

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(device->graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]->fence) != VK_SUCCESS) {
        throw std::runtime_error("Failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapchains[] = {
        swapchain->swapchain
    };

    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapchains;
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
    graphicsPipeline = new GraphicsPipeline(device->device, descriptorSetLayout->descriptorSetLayout, swapchain->swapchainExtent, renderPass->renderPass, device->msaaSamples);
    depthResources = new DepthResources(device->physicalDevice, device->device, swapchain->swapchainExtent, device->msaaSamples, commandPool->commandPool, device->graphicsQueue);
    colorResources = new ColorResources(device->physicalDevice, device->device, swapchain->swapchainExtent, device->msaaSamples, swapchain->swapchainImageFormat);

    framebuffers.resize(swapchain->swapchainImageViews.size());
    for (size_t i = 0; i < framebuffers.size(); i++) {
        std::array<VkImageView, 2> attachments = {
            swapchain->swapchainImageViews[i],
            depthResources->image->imageView
        };
        framebuffers[i] = new Framebuffer(device->device, renderPass->renderPass, static_cast<uint32_t>(attachments.size()), attachments.data(), swapchain->swapchainExtent);
    }

    camera->aspectRatio = (float) swapchain->swapchainExtent.width / (float) swapchain->swapchainExtent.height;
    ubo.projectionMatrix = camera->createProjectionMatrix();
}

void Renderer::cleanUpSwapchain() {
    for (auto framebuffer : framebuffers) {
        delete framebuffer;
    }
    framebuffers.clear();

    delete colorResources;
    delete depthResources;
    delete graphicsPipeline;
    delete renderPass;
    delete swapchain;
}

void Renderer::updateUniformBuffer(Entity* entity, uint32_t currentFrame) {
    ubo.modelMatrix = entity->createModelMatrix();

    void* data;
    vkMapMemory(device->device, uniformBuffers[currentFrame]->bufferMemory, 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device->device, uniformBuffers[currentFrame]->bufferMemory);
}

void Renderer::recordCommandBuffer(uint32_t currentFrame, uint32_t imageIndex, Mesh* mesh) {
    VkCommandBuffer commandBuffer = commandBuffers->commandBuffers[currentFrame];

    VkCommandBufferBeginInfo commandBufferBeginInfo{};
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo) != VK_SUCCESS) {
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

    vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->graphicsPipeline);

    VkBuffer vertexBuffers[] = {
        mesh->vertexBuffer->buffer
    };
    VkDeviceSize offsets[] = {
        0
    };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(commandBuffer, mesh->indexBuffer->buffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->pipelineLayout, 0, 1, &descriptorSets->descriptorSets[currentFrame], 0, nullptr);

    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(mesh->indicesSize), 1, 0, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer!");
    }
}
