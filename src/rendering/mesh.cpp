#include "mesh.hpp"
#include "renderer.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint16_t> indices, Renderer* renderer) {
    this->indicesSize = indices.size();

    // Vertex buffer
    VkDeviceSize vertexBufferSize = sizeof(vertices[0]) * vertices.size();
    Buffer* stagingBufferForVertexBuffer = new Buffer(renderer->device->physicalDevice, renderer->device->device, vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    void* stagingBufferForVertexBufferData;
    vkMapMemory(renderer->device->device, stagingBufferForVertexBuffer->bufferMemory, 0, vertexBufferSize, 0, &stagingBufferForVertexBufferData);
    memcpy(stagingBufferForVertexBufferData, vertices.data(), (size_t) vertexBufferSize);
    vkUnmapMemory(renderer->device->device, stagingBufferForVertexBuffer->bufferMemory);

    vertexBuffer = new Buffer(renderer->device->physicalDevice, renderer->device->device, vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    Buffer::Copy(renderer->device->device, renderer->commandPool->commandPool, renderer->device->graphicsQueue, stagingBufferForVertexBuffer->buffer, vertexBuffer->buffer, vertexBufferSize);
    delete stagingBufferForVertexBuffer;

    // Index buffer
    VkDeviceSize indexBufferSize = sizeof(indices[0]) * indices.size();
    Buffer* stagingBufferForIndexBuffer = new Buffer(renderer->device->physicalDevice, renderer->device->device, indexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    void* stagingBufferForIndexBufferData;
    vkMapMemory(renderer->device->device, stagingBufferForIndexBuffer->bufferMemory, 0, indexBufferSize, 0, &stagingBufferForIndexBufferData);
    memcpy(stagingBufferForIndexBufferData, indices.data(), (size_t) indexBufferSize);
    vkUnmapMemory(renderer->device->device, stagingBufferForIndexBuffer->bufferMemory);

    indexBuffer = new Buffer(renderer->device->physicalDevice, renderer->device->device, indexBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    Buffer::Copy(renderer->device->device, renderer->commandPool->commandPool, renderer->device->graphicsQueue, stagingBufferForIndexBuffer->buffer, indexBuffer->buffer, indexBufferSize);
    delete stagingBufferForIndexBuffer;
}

Mesh::~Mesh() {
    delete vertexBuffer;
    delete indexBuffer;
}
