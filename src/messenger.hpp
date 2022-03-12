#ifndef messenger_hpp
#define messenger_hpp

#include <vulkan/vulkan.h>
#include <stdexcept>
#include "app_info.hpp"
#include <iostream>

class Messenger {
public:
    VkDebugUtilsMessengerEXT messenger;
    Messenger(VkInstance& instance);
    ~Messenger();
private:
    VkInstance instance;
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
};

#endif
