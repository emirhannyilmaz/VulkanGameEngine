#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const std::string& fileName, float reflectivity, float shineDamper, Renderer* renderer) {
    this->reflectivity = reflectivity;
    this->shineDamper = shineDamper;

    int width;
    int height;
    int channels;
    uint32_t mipLevels;

    stbi_uc* pixels = stbi_load(fileName.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if (!pixels) {
        throw std::runtime_error("Failed to load texture!");
    }

    mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;

    VkDeviceSize size = width * height * 4;

    Buffer* stagingBuffer = new Buffer(renderer->device->physicalDevice, renderer->device->device, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    void* data;
    vkMapMemory(renderer->device->device, stagingBuffer->bufferMemory, 0, size, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(size));
    vkUnmapMemory(renderer->device->device, stagingBuffer->bufferMemory);

    stbi_image_free(pixels);

    image = new Image(renderer->device->physicalDevice, renderer->device->device, width, height, mipLevels, 1, VK_FORMAT_R8G8B8A8_SRGB,  VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_SAMPLE_COUNT_1_BIT, 0, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
    sampler = new Sampler(renderer->device->physicalDevice, renderer->device->device, mipLevels);

    Image::TransitionImageLayout(renderer->device->device, renderer->commandPool->commandPool, renderer->device->graphicsQueue, image->image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels, 1, VK_FORMAT_R8G8B8A8_SRGB);
    Image::CopyBufferToImage(renderer->device->device, renderer->commandPool->commandPool, renderer->device->graphicsQueue, static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1, stagingBuffer->buffer, image->image);
    Image::GenerateMipmaps(renderer->device->physicalDevice, renderer->device->device, renderer->commandPool->commandPool, renderer->device->graphicsQueue, image->image, VK_FORMAT_R8G8B8A8_SRGB, width, height, mipLevels);

    delete stagingBuffer;
}

Texture::Texture(const std::vector<std::string>& fileNames, Renderer* renderer) {
    int width;
    int height;
    int channels;
    std::array<stbi_uc*, 6> pixels{};

    for (size_t i = 0; i < pixels.size(); i++) {
        pixels[i] = stbi_load(fileNames[i].c_str(), &width, &height, &channels, STBI_rgb_alpha);
        if (!pixels[i]) {
            throw std::runtime_error("Failed to load texture!");
        }
    }

    VkDeviceSize size = width * height * 4 * 6;
    VkDeviceSize layerSize = size / 6;

    Buffer* stagingBuffer = new Buffer(renderer->device->physicalDevice, renderer->device->device, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    void* data;
    vkMapMemory(renderer->device->device, stagingBuffer->bufferMemory, 0, size, 0, &data);
    for (size_t i = 0; i < pixels.size(); i++) {
        memcpy(reinterpret_cast<char*>(data) + static_cast<char>(layerSize * i), pixels[i], static_cast<size_t>(size));
    }
    vkUnmapMemory(renderer->device->device, stagingBuffer->bufferMemory);

    for (size_t i = 0; i < pixels.size(); i++) {
        stbi_image_free(pixels[i]);
    }

    image = new Image(renderer->device->physicalDevice, renderer->device->device, width, height, 1, 6, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_VIEW_TYPE_CUBE, VK_IMAGE_ASPECT_COLOR_BIT);
    sampler = new Sampler(renderer->device->physicalDevice, renderer->device->device, 1);

    Image::TransitionImageLayout(renderer->device->device, renderer->commandPool->commandPool, renderer->device->graphicsQueue, image->image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, 6, VK_FORMAT_R8G8B8A8_SRGB);
    Image::CopyBufferToImage(renderer->device->device, renderer->commandPool->commandPool, renderer->device->graphicsQueue, static_cast<uint32_t>(width), static_cast<uint32_t>(height), 6, stagingBuffer->buffer, image->image);
    Image::TransitionImageLayout(renderer->device->device, renderer->commandPool->commandPool, renderer->device->graphicsQueue, image->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 1, 6, VK_FORMAT_R8G8B8A8_SRGB);

    delete stagingBuffer;
}

Texture::~Texture() {
    delete sampler;
    delete image;
}
