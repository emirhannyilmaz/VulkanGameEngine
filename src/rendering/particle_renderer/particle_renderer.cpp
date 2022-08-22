#include "particle_renderer.hpp"

ParticleRenderer::ParticleRenderer(Renderer* renderer) {
    this->renderer = renderer;

    std::array<VkDescriptorSetLayoutBinding, 2> layoutBindings{};
    layoutBindings[0] = {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr};
    layoutBindings[1] = {1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
    descriptorSetLayout = new DescriptorSetLayout(renderer->device->device, static_cast<uint32_t>(layoutBindings.size()), layoutBindings.data());

    CreateGraphicsPipelines();

    std::array<VkDescriptorPoolSize, 1> poolSizes{};
    poolSizes[0] = {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2 * MAX_FRAMES_IN_FLIGHT};
    descriptorPool = new DescriptorPool(renderer->device->device, static_cast<uint32_t>(poolSizes.size()), poolSizes.data(), MAX_FRAMES_IN_FLIGHT);

    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout->descriptorSetLayout);
    descriptorSets = new DescriptorSets(renderer->device->device, descriptorPool->descriptorPool, layouts.data(), MAX_FRAMES_IN_FLIGHT);

    vertexUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    fragmentUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vertexUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(ParticleRendererVertexUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 0, 0, 1, vertexUniformBuffers[i]->buffer, sizeof(ParticleRendererVertexUniformBufferObject));

        fragmentUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(ParticleRendererFragmentUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 1, 0, 1, fragmentUniformBuffers[i]->buffer, sizeof(ParticleRendererFragmentUniformBufferObject));
    }
}

ParticleRenderer::~ParticleRenderer() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        delete vertexUniformBuffers[i];
        delete fragmentUniformBuffers[i];
    }
    vertexUniformBuffers.clear();
    fragmentUniformBuffers.clear();

    delete descriptorPool;
    DeleteGraphicsPipelines();
    delete descriptorSetLayout;
}

void ParticleRenderer::CreateGraphicsPipelines() {
    std::array<VkPushConstantRange, 2> pushConstantRanges{};
    pushConstantRanges[0] = {VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(ParticleVertexPushConstants)};
    pushConstantRanges[1] = {VK_SHADER_STAGE_VERTEX_BIT, sizeof(ParticleVertexPushConstants), sizeof(ParticleRendererVertexPushConstants)};

    graphicsPipeline = new GraphicsPipeline(renderer->device->device, "res/shaders/particle_shader.vert.spv", "res/shaders/particle_shader.frag.spv", 0, nullptr, 0, nullptr, 1, &descriptorSetLayout->descriptorSetLayout, static_cast<uint32_t>(pushConstantRanges.size()), pushConstantRanges.data(), renderer->swapchain->swapchainExtent, renderer->renderPass->renderPass, renderer->device->msaaSamples);
    offScreenGraphicsPipeline = new GraphicsPipeline(renderer->device->device, "res/shaders/particle_shader.vert.spv", "res/shaders/particle_shader.frag.spv", 0, nullptr, 0, nullptr, 1, &descriptorSetLayout->descriptorSetLayout, static_cast<uint32_t>(pushConstantRanges.size()), pushConstantRanges.data(), renderer->swapchain->swapchainExtent, renderer->waterResources->renderPass->renderPass, VK_SAMPLE_COUNT_1_BIT);
}

void ParticleRenderer::DeleteGraphicsPipelines() {
    delete graphicsPipeline;
    delete offScreenGraphicsPipeline;
}

void ParticleRenderer::render(std::vector<Particle*> particles, PerspectiveCamera* perspectiveCamera, glm::vec4 clipPlane, CommandBuffers* commandBuffers, bool onScreen) {
    updateDescriptorSetResources(perspectiveCamera);
    updatePushConstants(clipPlane);

    VkCommandBuffer commandBuffer = commandBuffers->commandBuffers[renderer->currentFrame];
    GraphicsPipeline* gp = onScreen ? graphicsPipeline : offScreenGraphicsPipeline;

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, gp->graphicsPipeline);
    vkCmdPushConstants(commandBuffer, gp->pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, sizeof(ParticleVertexPushConstants), sizeof(ParticleRendererVertexPushConstants), &vertexPushConstants);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, gp->pipelineLayout, 0, 1, &descriptorSets->descriptorSets[renderer->currentFrame], 0, nullptr);
    
    for (Particle* particle : particles) {
        particle->updatePushConstants(perspectiveCamera->createViewMatrix());
        vkCmdPushConstants(commandBuffer, gp->pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(ParticleVertexPushConstants), &particle->vertexPushConstants);
        vkCmdDraw(commandBuffer, 6, 1, 0, 0);
    }
}

void ParticleRenderer::updateDescriptorSetResources(PerspectiveCamera* perspectiveCamera) {
    ParticleRendererVertexUniformBufferObject vertexUbo{};
    vertexUbo.projectionMatrix = perspectiveCamera->createProjectionMatrix();
    void* vubData;
    vkMapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory, 0, sizeof(vertexUbo), 0, &vubData);
    memcpy(vubData, &vertexUbo, sizeof(vertexUbo));
    vkUnmapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory);

    ParticleRendererFragmentUniformBufferObject fragmentUbo{};
    fragmentUbo.fogColor = FOG_COLOR;
    void* fubData;
    vkMapMemory(renderer->device->device, fragmentUniformBuffers[renderer->currentFrame]->bufferMemory, 0, sizeof(fragmentUbo), 0, &fubData);
    memcpy(fubData, &fragmentUbo, sizeof(fragmentUbo));
    vkUnmapMemory(renderer->device->device, fragmentUniformBuffers[renderer->currentFrame]->bufferMemory);
}

void ParticleRenderer::updatePushConstants(glm::vec4 clipPlane) {
    vertexPushConstants.clipPlane = clipPlane;
}
