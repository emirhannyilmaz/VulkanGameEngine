#ifndef shader_module_hpp
#define shader_module_hpp

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

class ShaderModule {
public:
    VkShaderModule shaderModule;
    ShaderModule(VkDevice& device, const std::string& fileName);
    ~ShaderModule();
private:
    VkDevice device;
};

#endif
