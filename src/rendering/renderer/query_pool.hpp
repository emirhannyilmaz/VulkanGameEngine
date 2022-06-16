#ifndef query_pool_hpp
#define query_pool_hpp

#include <vulkan/vulkan.h>
#include <stdexcept>

class QueryPool {
public:
    VkQueryPool queryPool;
    QueryPool(VkDevice& device, uint32_t queryCount);
    ~QueryPool();
private:
    VkDevice device;
};

#endif
