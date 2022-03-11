#include "app.hpp"

void App::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanUp();
}

void App::initWindow() {
    window = new Window();
}

void App::initVulkan() {
    instance = new Instance();

    if (enableValidationLayers) {
        messenger = new Messenger(instance->instance);
    }

    surface = new Surface(instance->instance, window->window);
    device = new Device(instance->instance, surface->surface);
    swapchain = new Swapchain(device->device, surface->surface, device->swapchainSupportDetails, window->window, device->indices);
    renderPass = new RenderPass(device->device, swapchain->swapchainImageFormat);
    graphicsPipeline = new GraphicsPipeline(device->device, swapchain->swapchainExtent, renderPass->renderPass);

    framebuffers.resize(swapchain->swapchainImageViews.size());
    for (size_t i = 0; i < framebuffers.size(); i++) {
        VkImageView attachments[] = {
            swapchain->swapchainImageViews[i]
        };
        framebuffers[i] = new Framebuffer(device->device, renderPass->renderPass, attachments, swapchain->swapchainExtent);
    }

    commandPool = new CommandPool(device->device, device->indices.graphicsFamily.value(), VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

    // Vertex buffer
    VkDeviceSize vertexBufferSize = sizeof(vertices[0]) * vertices.size();
    Buffer* stagingBufferForVertexBuffer = new Buffer(device->physicalDevice, device->device, vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    void* stagingBufferForVertexBufferData;
    vkMapMemory(device->device, stagingBufferForVertexBuffer->bufferMemory, 0, vertexBufferSize, 0, &stagingBufferForVertexBufferData);
    memcpy(stagingBufferForVertexBufferData, vertices.data(), (size_t) vertexBufferSize);
    vkUnmapMemory(device->device, stagingBufferForVertexBuffer->bufferMemory);

    vertexBuffer = new Buffer(device->physicalDevice, device->device, vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    Buffer::Copy(device->device, commandPool->commandPool, device->graphicsQueue, stagingBufferForVertexBuffer->buffer, vertexBuffer->buffer, vertexBufferSize);
    delete stagingBufferForVertexBuffer;

    // Index buffer
    VkDeviceSize indexBufferSize = sizeof(indices[0]) * indices.size();
    Buffer* stagingBufferForIndexBuffer = new Buffer(device->physicalDevice, device->device, indexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    void* stagingBufferForIndexBufferData;
    vkMapMemory(device->device, stagingBufferForIndexBuffer->bufferMemory, 0, indexBufferSize, 0, &stagingBufferForIndexBufferData);
    memcpy(stagingBufferForIndexBufferData, indices.data(), (size_t) indexBufferSize);
    vkUnmapMemory(device->device, stagingBufferForIndexBuffer->bufferMemory);

    indexBuffer = new Buffer(device->physicalDevice, device->device, indexBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    Buffer::Copy(device->device, commandPool->commandPool, device->graphicsQueue, stagingBufferForIndexBuffer->buffer, indexBuffer->buffer, indexBufferSize);
    delete stagingBufferForIndexBuffer;

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

void App::mainLoop() {
    while (!glfwWindowShouldClose(window->window)) {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(device->device);
}

void App::drawFrame() {
    vkWaitForFences(device->device, 1, &inFlightFences[currentFrame]->fence, VK_TRUE, UINT64_MAX);

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
    recordCommandBuffer(commandBuffers->commandBuffers[currentFrame], imageIndex);

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

void App::recreateSwapchain() {
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
    renderPass = new RenderPass(device->device, swapchain->swapchainImageFormat);
    graphicsPipeline = new GraphicsPipeline(device->device, swapchain->swapchainExtent, renderPass->renderPass);
    framebuffers.resize(swapchain->swapchainImageViews.size());
    for (size_t i = 0; i < framebuffers.size(); i++) {
        VkImageView attachments[] = {
            swapchain->swapchainImageViews[i]
        };
        framebuffers[i] = new Framebuffer(device->device, renderPass->renderPass, attachments, swapchain->swapchainExtent);
    }
}

void App::cleanUpSwapchain() {
    for (auto framebuffer : framebuffers) {
        delete framebuffer;
    }
    framebuffers.clear();
    delete graphicsPipeline;
    delete renderPass;
    delete swapchain;
}

void App::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
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
    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassBeginInfo.clearValueCount = 1;
    renderPassBeginInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->graphicsPipeline);

    VkBuffer vertexBuffers[] = {
        vertexBuffer->buffer
    };
    VkDeviceSize offsets[] = {
        0
    };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(commandBuffer, indexBuffer->buffer, 0, VK_INDEX_TYPE_UINT16);

    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer!");
    }
}

void App::cleanUp() {
    cleanUpSwapchain();

    delete indexBuffer;
    delete vertexBuffer;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        delete imageAvailableSemaphores[i];
        delete renderFinishedSemaphores[i];
        delete inFlightFences[i];
    }
    imageAvailableSemaphores.clear();
    renderFinishedSemaphores.clear();
    inFlightFences.clear();

    delete commandBuffers;
    delete commandPool;
    delete device;

    if (enableValidationLayers) {
        delete messenger;
    }

    delete surface;
    delete instance;
    delete window;
}
