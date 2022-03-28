#include "texture.hpp"

Texture::Texture(const std::string& fileName, Renderer* renderer) {
    int width;
    int height;
    int channels;

    stbi_uc* pixels = stbi_load(fileName.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    VkDeviceSize size = width * height * 4;

    if (!pixels) {
        throw std::runtime_error("Failed to load texture!");
    }

    Buffer* stagingBuffer = new Buffer(renderer->physicalDevice, renderer->device, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    void* data;
    vkMapMemory(renderer->device, stagingBuffer->bufferMemory, 0, size, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(size));
    vkUnmapMemory(renderer->device, stagingBuffer->bufferMemory);

    stbi_image_free(pixels);

    image = new Image(renderer->physicalDevice, renderer->device, width, height, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    Image::TransitionImageLayout(renderer->device, renderer->commandPool, renderer->device->graphicsQueue, image->image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    Image::CopyBufferToImage(renderer->device, renderer->commandPool, renderer->device->graphicsQueue, stagingBuffer->buffer, image->image, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
    Image::TransitionImageLayout(renderer->device, renderer->commandPool, renderer->device->graphicsQueue, image->image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    delete stagingBuffer;
}

Texture::~Texture() {
    delete image;
}
