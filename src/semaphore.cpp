#include "semaphore.hpp"

Semaphore::Semaphore(VkDevice device) {
    this->device = device;

    VkSemaphoreCreateInfo semaphoreCreateInfo{};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &semaphore) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create semaphore!");
    }
}

Semaphore::~Semaphore() {
    vkDestroySemaphore(device, semaphore, nullptr);
}
