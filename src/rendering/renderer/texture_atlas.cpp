#include "texture_atlas.hpp"

TextureAtlas::TextureAtlas(const std::string& fileName, int rowCount, Renderer* renderer) {
    this->rowCount = rowCount;
    imageCount = rowCount * rowCount;

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

TextureAtlas::~TextureAtlas() {
    delete sampler;
    delete image;
}

glm::vec2 TextureAtlas::getOffsetAtIndex(int index) {
    int column = fmod(index, rowCount);
    int row = index / rowCount;
    return glm::vec2((float) column / rowCount, (float) row / rowCount);
}
