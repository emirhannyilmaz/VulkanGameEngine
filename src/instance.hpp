#ifndef instance_hpp
#define instance_hpp

#include <vulkan/vulkan.h>
#include "window.hpp"
#include <vector>
#include <stdexcept>
#include "app_info.hpp"
#include <iostream>

class Instance {
public:
    VkInstance instance;
    Instance();
    ~Instance();
private:
    bool checkValidationLayerSupport();
    std::vector<const char*> getRequiredExtensions();
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
};

#endif
