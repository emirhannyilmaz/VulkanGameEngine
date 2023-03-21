#include "render_pass.hpp"

RenderPass::RenderPass(VkDevice& device) {
    this->device = device;
}

RenderPass::~RenderPass() {
    vkDestroyRenderPass(device, renderPass, nullptr);
}
