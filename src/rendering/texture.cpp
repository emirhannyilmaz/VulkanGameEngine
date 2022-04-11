#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const std::string& fileName, Renderer* renderer) {
    int width;
    int height;
    int channels;
    uint32_t mipLevels;

    stbi_uc* pixels = stbi_load(fileName.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;

    VkDeviceSize size = width * height * 4;

    if (!pixels) {
        throw std::runtime_error("Failed to load texture!");
    }

    Buffer* stagingBuffer = new Buffer(renderer->device->physicalDevice, renderer->device->device, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    void* data;
    vkMapMemory(renderer->device->device, stagingBuffer->bufferMemory, 0, size, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(size));
    vkUnmapMemory(renderer->device->device, stagingBuffer->bufferMemory);

    stbi_image_free(pixels);

    image = new Image(renderer->device->physicalDevice, renderer->device->device, width, height, mipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT);

    Image::TransitionImageLayout(renderer->device->device, renderer->commandPool->commandPool, renderer->device->graphicsQueue, image->image, mipLevels, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    Image::CopyBufferToImage(renderer->device->device, renderer->commandPool->commandPool, renderer->device->graphicsQueue, stagingBuffer->buffer, image->image, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
    Image::GenerateMipmaps(renderer->device->physicalDevice, renderer->device->device, renderer->commandPool->commandPool, renderer->device->graphicsQueue, image->image, VK_FORMAT_R8G8B8A8_SRGB, width, height, mipLevels);

    delete stagingBuffer;
}

Texture::~Texture() {
    delete image;
}
