#include "shadow_map_resources.hpp"

ShadowMapResources::ShadowMapResources(VkPhysicalDevice& physicalDevice, VkDevice& device, VkExtent2D& extent) {
    depthResources = new DepthResources(physicalDevice, device, extent, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
    sampler = new Sampler(physicalDevice, device, 0);
    renderPass = new ShadowMapRendererRenderPass(device, DepthResources::findDepthFormat(physicalDevice));

    std::array<VkImageView, 1> attachments = {
        depthResources->image->imageView
    };
    framebuffer = new Framebuffer(device, renderPass->renderPass, static_cast<uint32_t>(attachments.size()), attachments.data(), extent);
}

ShadowMapResources::~ShadowMapResources() {
    delete framebuffer;
    delete renderPass;
    delete sampler;
    delete depthResources;
}
