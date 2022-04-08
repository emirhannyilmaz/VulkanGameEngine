#include "framebuffer.hpp"

Framebuffer::Framebuffer(VkDevice& device, VkRenderPass& renderPass, uint32_t attachmentCount, VkImageView* attachments, VkExtent2D& extent) {
    this->device = device;

    VkFramebufferCreateInfo framebufferCreateInfo{};
    framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferCreateInfo.renderPass = renderPass;
    framebufferCreateInfo.attachmentCount = attachmentCount;
    framebufferCreateInfo.pAttachments = attachments;
    framebufferCreateInfo.width = extent.width;
    framebufferCreateInfo.height = extent.height;
    framebufferCreateInfo.layers = 1;

    if (vkCreateFramebuffer(device, &framebufferCreateInfo, nullptr, &framebuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create framebuffer!");
    }
}

Framebuffer::~Framebuffer() {
    vkDestroyFramebuffer(device, framebuffer, nullptr);
}
