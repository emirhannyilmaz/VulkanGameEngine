#include "query_pool.hpp"

QueryPool::QueryPool(VkDevice& device, uint32_t queryCount) {
    this->device = device;

    VkQueryPoolCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
    createInfo.queryType = VK_QUERY_TYPE_TIMESTAMP;
    createInfo.queryCount = queryCount;

    if (vkCreateQueryPool(device, &createInfo, nullptr, &queryPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create query pool!");
    }
}

QueryPool::~QueryPool() {
    vkDestroyQueryPool(device, queryPool, nullptr);
}
