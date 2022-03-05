#ifndef shader_module_hpp
#define shader_module_hpp

#define VK_ENABLE_BETA_EXTENSIONS
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <fstream>

class ShaderModule {
public:
    VkShaderModule shaderModule;
    ShaderModule(VkDevice device, const std::string& fileName);
    ~ShaderModule();
private:
    VkDevice device;
};

#endif
