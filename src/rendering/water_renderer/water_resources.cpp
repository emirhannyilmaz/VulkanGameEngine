#include "water_resources.hpp"

WaterResources::WaterResources(VkPhysicalDevice& physicalDevice, VkDevice& device, VkExtent2D& extent, VkFormat colorAttachmentFormat) {
    reflectionColorResources = new ColorResources(physicalDevice, device, extent, VK_SAMPLE_COUNT_1_BIT, colorAttachmentFormat, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
    reflectionDepthResources = new DepthResources(physicalDevice, device, extent, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
    refractionColorResources = new ColorResources(physicalDevice, device, extent, VK_SAMPLE_COUNT_1_BIT, colorAttachmentFormat, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
    refractionDepthResources = new DepthResources(physicalDevice, device, extent, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
    sampler = new Sampler(physicalDevice, device, 0);
    renderPass = new RenderPass(device, colorAttachmentFormat, DepthResources::findDepthFormat(physicalDevice), VK_SAMPLE_COUNT_1_BIT, false, true);

    std::array<VkImageView, 2> reflectionAttachments = {
        reflectionColorResources->image->imageView,
        reflectionDepthResources->image->imageView
    };
    reflectionFramebuffer = new Framebuffer(device, renderPass->renderPass, static_cast<uint32_t>(reflectionAttachments.size()), reflectionAttachments.data(), extent);

    std::array<VkImageView, 2> refractionAttachments = {
        refractionColorResources->image->imageView,
        refractionDepthResources->image->imageView
    };
    refractionFramebuffer = new Framebuffer(device, renderPass->renderPass, static_cast<uint32_t>(refractionAttachments.size()), refractionAttachments.data(), extent);
}

WaterResources::~WaterResources() {
    delete refractionFramebuffer;
    delete reflectionFramebuffer;
    delete renderPass;
    delete sampler;
    delete refractionDepthResources;
    delete refractionColorResources;
    delete reflectionDepthResources;
    delete reflectionColorResources;
}
