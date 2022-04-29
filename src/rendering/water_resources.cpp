#include "water_resources.hpp"

WaterResources::WaterResources(VkPhysicalDevice& physicalDevice, VkDevice& device, VkExtent2D& extent, VkFormat colorAttachmentFormat, VkCommandPool& commandPool, VkQueue& graphicsQueue) {
    colorResources = new ColorResources(physicalDevice, device, extent, VK_SAMPLE_COUNT_1_BIT, colorAttachmentFormat, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
    depthResources = new DepthResources(physicalDevice, device, extent, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
    sampler = new Sampler(physicalDevice, device, 0);
    renderPass = new RenderPass(device, colorAttachmentFormat, DepthResources::findDepthFormat(physicalDevice), VK_SAMPLE_COUNT_1_BIT, false);

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
    delete sampler;
    delete depthResources;
    delete colorResources;
}
