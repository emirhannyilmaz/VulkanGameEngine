#ifndef app_hpp
#define app_hpp

#include <string>
#include <vector>
#include <iostream>
#include <optional>
#include <set>
#include <cstdint>
#include <algorithm>
#include <fstream>

const int MAX_FRAMES_IN_FLIGHT = 2;

class App {
public:
    void run();
private:
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame = 0;
    void initVulkan();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createCommandPool();
    void createCommandBuffers();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void createSyncObjects();
    void cleanUpSwapChain();
    void recreateSwapChain();
    std::vector<char> readFile(const std::string& fileName);
    VkShaderModule createShaderModule(const std::vector<char>& code);
    void mainLoop();
    void drawFrame();
    void cleanUp();
};

#endif
