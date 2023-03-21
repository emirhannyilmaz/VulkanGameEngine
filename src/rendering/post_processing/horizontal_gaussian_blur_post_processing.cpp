#include "horizontal_gaussian_blur_post_processing.hpp"

HorizontalGaussianBlurPostProcessing::HorizontalGaussianBlurPostProcessing(Renderer* renderer) {
    this->renderer = renderer;

    std::array<VkDescriptorSetLayoutBinding, 2> layoutBindings{};
    layoutBindings[0] = {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr};
    layoutBindings[1] = {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
    descriptorSetLayout = new DescriptorSetLayout(renderer->device->device, static_cast<uint32_t>(layoutBindings.size()), layoutBindings.data());

    CreateGraphicsPipeline();

    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0] = {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_FRAMES_IN_FLIGHT};
    poolSizes[1] = {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_FRAMES_IN_FLIGHT};
    descriptorPool = new DescriptorPool(renderer->device->device, static_cast<uint32_t>(poolSizes.size()), poolSizes.data(), MAX_FRAMES_IN_FLIGHT);

    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout->descriptorSetLayout);
    descriptorSets = new DescriptorSets(renderer->device->device, descriptorPool->descriptorPool, layouts.data(), MAX_FRAMES_IN_FLIGHT);

    vertexUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vertexUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(HorizontalGaussianBlurVertexUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 0, 0, 1, vertexUniformBuffers[i]->buffer, sizeof(HorizontalGaussianBlurVertexUniformBufferObject));
        
        updateDescriptorSetImageInfos();
    }
}

HorizontalGaussianBlurPostProcessing::~HorizontalGaussianBlurPostProcessing() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        delete vertexUniformBuffers[i];
    }
    vertexUniformBuffers.clear();

    delete descriptorPool;
    DeleteGraphicsPipeline();
    delete descriptorSetLayout;
}

void HorizontalGaussianBlurPostProcessing::CreateGraphicsPipeline() {
    std::array<VkDescriptorSetLayout, 1> descriptorSetLayouts{};
    descriptorSetLayouts[0] = descriptorSetLayout->descriptorSetLayout;

    graphicsPipeline = new GraphicsPipeline(renderer->device->device, "res/shaders/horizontal_gaussian_blur_shader.vert.spv", "res/shaders/common_gaussian_blur_shader.frag.spv", 0, nullptr, 0, nullptr, static_cast<uint32_t>(descriptorSetLayouts.size()), descriptorSetLayouts.data(), 0, nullptr, renderer->swapchain->swapchainExtent, renderer->gaussianBlurResources->horizontalBlurRenderPass->renderPass, VK_SAMPLE_COUNT_1_BIT, VK_FALSE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA);
}

void HorizontalGaussianBlurPostProcessing::DeleteGraphicsPipeline() {
    delete graphicsPipeline;
}

void HorizontalGaussianBlurPostProcessing::process(float imageWidth, CommandBuffers* commandBuffers) {
    updateDescriptorSetResources(imageWidth);

    VkCommandBuffer commandBuffer = commandBuffers->commandBuffers[renderer->currentFrame];

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->graphicsPipeline);

    std::array<VkDescriptorSet, 1> descriptorSetsToBind{};
    descriptorSetsToBind[0] = descriptorSets->descriptorSets[renderer->currentFrame];
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->pipelineLayout, 0, static_cast<uint32_t>(descriptorSetsToBind.size()), descriptorSetsToBind.data(), 0, nullptr);
    vkCmdDraw(commandBuffer, 6, 1, 0, 0);
}

void HorizontalGaussianBlurPostProcessing::updateDescriptorSetImageInfos() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        descriptorSets->updateImageInfo(i, 1, 0, 1, renderer->gaussianBlurResources->rawColorResources->image->imageView, renderer->gaussianBlurResources->sampler->sampler);
    }
}

void HorizontalGaussianBlurPostProcessing::updateDescriptorSetResources(float imageWidth) {
    HorizontalGaussianBlurVertexUniformBufferObject vertexUbo{};
    vertexUbo.imageWidth = imageWidth;
    void* vubData;
    vkMapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory, 0, sizeof(vertexUbo), 0, &vubData);
    memcpy(vubData, &vertexUbo, sizeof(vertexUbo));
    vkUnmapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory);
}
