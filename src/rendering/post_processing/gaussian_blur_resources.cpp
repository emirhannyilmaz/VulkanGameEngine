#include "gaussian_blur_resources.hpp"

GaussianBlurResources::GaussianBlurResources(VkPhysicalDevice& physicalDevice, VkDevice& device, VkExtent2D& extent, VkFormat colorAttachmentFormat) {
    rawColorResources = new ColorResources(physicalDevice, device, extent, VK_SAMPLE_COUNT_1_BIT, colorAttachmentFormat, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
    rawDepthResources = new DepthResources(physicalDevice, device, extent, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
    horizontalBlurColorResources = new ColorResources(physicalDevice, device, extent, VK_SAMPLE_COUNT_1_BIT, colorAttachmentFormat, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);

    sampler = new Sampler(physicalDevice, device, 0);

    rawRenderPass = new RenderPass(device, colorAttachmentFormat, DepthResources::findDepthFormat(physicalDevice), VK_SAMPLE_COUNT_1_BIT, false, true, true, false);
    horizontalBlurRenderPass = new RenderPass(device, colorAttachmentFormat, DepthResources::findDepthFormat(physicalDevice), VK_SAMPLE_COUNT_1_BIT, false, true, false, false);

    std::array<VkImageView, 2> rawAttachments = {
        rawColorResources->image->imageView,
        rawDepthResources->image->imageView
    };
    rawFramebuffer = new Framebuffer(device, rawRenderPass->renderPass, static_cast<uint32_t>(rawAttachments.size()), rawAttachments.data(), extent);

    std::array<VkImageView, 1> horizontalBlurAttachments = {
        horizontalBlurColorResources->image->imageView
    };
    horizontalBlurFramebuffer = new Framebuffer(device, horizontalBlurRenderPass->renderPass, static_cast<uint32_t>(horizontalBlurAttachments.size()), horizontalBlurAttachments.data(), extent);
}

GaussianBlurResources::~GaussianBlurResources() {
    delete horizontalBlurFramebuffer;
    delete rawFramebuffer;
    delete horizontalBlurRenderPass;
    delete rawRenderPass;
    delete sampler;
    delete horizontalBlurColorResources;
    delete rawDepthResources;
    delete rawColorResources;
}
