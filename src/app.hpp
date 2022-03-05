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
    uint32_t currentFrame = 0;
    void initVulkan();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void mainLoop();
    void drawFrame();
    void cleanUp();
};

#endif
