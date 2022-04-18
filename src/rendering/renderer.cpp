#include "renderer.hpp"

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
    std::array<VkDescriptorSetLayoutBinding, 2> layoutBindings{};
    layoutBindings[0] = {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr};
    layoutBindings[1] = {1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
    descriptorSetLayout = new DescriptorSetLayout(device->device, static_cast<uint32_t>(layoutBindings.size()), layoutBindings.data());
    Entity::CreateDesriptorSetLayout(device->device);
    std::array<VkDescriptorSetLayout, 2> descriptorSetLayouts{};
    descriptorSetLayouts[0] = descriptorSetLayout->descriptorSetLayout;
    descriptorSetLayouts[1] = Entity::descriptorSetLayout->descriptorSetLayout;
    graphicsPipeline = new GraphicsPipeline(device->device, static_cast<uint32_t>(descriptorSetLayouts.size()), descriptorSetLayouts.data(), swapchain->swapchainExtent, renderPass->renderPass, device->msaaSamples);
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
    std::array<VkDescriptorPoolSize, 1> poolSizes{};
    poolSizes[0] = {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2 * MAX_FRAMES_IN_FLIGHT};
    descriptorPool = new DescriptorPool(device->device, static_cast<uint32_t>(poolSizes.size()), poolSizes.data(), MAX_FRAMES_IN_FLIGHT);
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout->descriptorSetLayout);
    descriptorSets = new DescriptorSets(device->device, descriptorPool->descriptorPool, layouts.data(), MAX_FRAMES_IN_FLIGHT);
    vertexUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    fragmentUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vertexUniformBuffers[i] = new Buffer(device->physicalDevice, device->device, sizeof(GeneralVertexUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 0, 0, 1, vertexUniformBuffers[i]->buffer, sizeof(GeneralVertexUniformBufferObject));

        fragmentUniformBuffers[i] = new Buffer(device->physicalDevice, device->device, sizeof(GeneralFragmentUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 1, 0, 1, fragmentUniformBuffers[i]->buffer, sizeof(GeneralFragmentUniformBufferObject));

        vertexUbo.projectionMatrix = camera->createProjectionMatrix();
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
    Entity::DeleteDesriptorSetLayout();
    cleanUpSwapchain();
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        delete vertexUniformBuffers[i];
        delete fragmentUniformBuffers[i];
    }
    vertexUniformBuffers.clear();
    fragmentUniformBuffers.clear();
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

void Renderer::render(std::vector<Entity*> entities, Light* light) {
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

    vkResetFences(device->device, 1, &inFlightFences[currentFrame]->fence);
    vkResetCommandBuffer(commandBuffers->commandBuffers[currentFrame], 0);

    recordCommandBuffer(currentFrame, imageIndex, entities, light);

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
    std::array<VkDescriptorSetLayout, 2> descriptorSetLayouts{};
    descriptorSetLayouts[0] = descriptorSetLayout->descriptorSetLayout;
    descriptorSetLayouts[1] = Entity::descriptorSetLayout->descriptorSetLayout;
    graphicsPipeline = new GraphicsPipeline(device->device, static_cast<uint32_t>(descriptorSetLayouts.size()), descriptorSetLayouts.data(), swapchain->swapchainExtent, renderPass->renderPass, device->msaaSamples);
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
    vertexUbo.projectionMatrix = camera->createProjectionMatrix();
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

void Renderer::recordCommandBuffer(uint32_t currentFrame, uint32_t imageIndex, std::vector<Entity*> entities, Light* light) {
    vertexUbo.viewMatrix = camera->createViewMatrix();
    vertexUbo.lightPosition = light->position;
    void* vubData;
    vkMapMemory(device->device, vertexUniformBuffers[currentFrame]->bufferMemory, 0, sizeof(vertexUbo), 0, &vubData);
    memcpy(vubData, &vertexUbo, sizeof(vertexUbo));
    vkUnmapMemory(device->device, vertexUniformBuffers[currentFrame]->bufferMemory);

    GeneralFragmentUniformBufferObject fragmentUbo{};
    fragmentUbo.lightColor = light->color;
    void* fubData;
    vkMapMemory(device->device, fragmentUniformBuffers[currentFrame]->bufferMemory, 0, sizeof(fragmentUbo), 0, &fubData);
    memcpy(fubData, &fragmentUbo, sizeof(fragmentUbo));
    vkUnmapMemory(device->device, fragmentUniformBuffers[currentFrame]->bufferMemory);

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
    std::array<VkDescriptorSet, 2> descriptorSetsToBind{};
    descriptorSetsToBind[0] = descriptorSets->descriptorSets[currentFrame];
    for (Entity* entity : entities) {
        entity->updateDescriptorSetResources(currentFrame);

        VkDeviceSize offsets = 0;
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, &entity->mesh->vertexBuffer->buffer, &offsets);
        vkCmdBindIndexBuffer(commandBuffer, entity->mesh->indexBuffer->buffer, 0, VK_INDEX_TYPE_UINT32);
        descriptorSetsToBind[1] = entity->descriptorSets->descriptorSets[currentFrame];
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->pipelineLayout, 0, static_cast<uint32_t>(descriptorSetsToBind.size()), descriptorSetsToBind.data(), 0, nullptr);

        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(entity->mesh->indicesSize), 1, 0, 0, 0);
    }

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer!");
    }
}
