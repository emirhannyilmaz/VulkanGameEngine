#include "water_resources.hpp"

WaterResources::WaterResources(VkPhysicalDevice& physicalDevice, VkDevice& device, VkExtent2D& extent, VkSampleCountFlagBits msaaSamples, VkFormat colorAttachmentFormat, VkCommandPool& commandPool, VkQueue& graphicsQueue) {
    colorResources = new ColorResources(physicalDevice, device, extent, msaaSamples, colorAttachmentFormat);
    depthResources = new DepthResources(physicalDevice, device, extent, msaaSamples, commandPool, graphicsQueue);
    renderPass = new RenderPass(device, colorAttachmentFormat, DepthResources::findDepthFormat(physicalDevice), msaaSamples, false);

    std::array<VkImageView, 2> attachments = {
        colorResources->image->imageView,
        depthResources->image->imageView,
    };
    reflectionFramebuffer = new Framebuffer(device, renderPass->renderPass, static_cast<uint32_t>(attachments.size()), attachments.data(), extent);
    refractionFramebuffer = new Framebuffer(device, renderPass->renderPass, static_cast<uint32_t>(attachments.size()), attachments.data(), extent);
}

WaterResources::~WaterResources() {
    delete refractionFramebuffer;
    delete reflectionFramebuffer;
    delete renderPass;
    delete depthResources;
    delete colorResources;
}
