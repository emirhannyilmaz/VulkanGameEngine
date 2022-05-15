#include "terrain.hpp"
#include "../rendering/texture.hpp"
#include "../rendering/renderer.hpp"

DescriptorSetLayout* Terrain::descriptorSetLayout = nullptr;

Terrain::Terrain(Texture* texture, const std::string& heightMapFileName, glm::vec2 position, Renderer* renderer) {
    this->texture = texture;
    this->position = position * SIZE;
    this->renderer = renderer;
    createMesh(heightMapFileName);

    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0] = {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2 * MAX_FRAMES_IN_FLIGHT};
    poolSizes[1] = {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_FRAMES_IN_FLIGHT};
    descriptorPool = new DescriptorPool(renderer->device->device, static_cast<uint32_t>(poolSizes.size()), poolSizes.data(), MAX_FRAMES_IN_FLIGHT);

    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout->descriptorSetLayout);
    descriptorSets = new DescriptorSets(renderer->device->device, descriptorPool->descriptorPool, layouts.data(), MAX_FRAMES_IN_FLIGHT);

    TerrainFragmentUniformBufferObject ubo{};
    ubo.reflectivity = texture->reflectivity;
    ubo.shineDamper = texture->shineDamper;

    vertexUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    fragmentUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vertexUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(TerrainVertexUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 0, 0, 1, vertexUniformBuffers[i]->buffer, sizeof(TerrainVertexUniformBufferObject));

        fragmentUniformBuffers[i] = new Buffer(renderer->device->physicalDevice, renderer->device->device, sizeof(TerrainFragmentUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        descriptorSets->updateBufferInfo(i, 1, 0, 1, fragmentUniformBuffers[i]->buffer, sizeof(TerrainFragmentUniformBufferObject));

        descriptorSets->updateImageInfo(i, 2, 0, 1, texture->image->imageView, texture->sampler->sampler);

        void* data;
        vkMapMemory(renderer->device->device, fragmentUniformBuffers[i]->bufferMemory, 0, sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(renderer->device->device, fragmentUniformBuffers[i]->bufferMemory);
    }
}

Terrain::~Terrain() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        delete vertexUniformBuffers[i];
        delete fragmentUniformBuffers[i];
    }
    vertexUniformBuffers.clear();
    fragmentUniformBuffers.clear();
    delete descriptorPool;
    delete mesh;
    delete texture;
}

void Terrain::updateDescriptorSetResources() {
    glm::mat4 matrix(1.0f);
    matrix = glm::translate(matrix, glm::vec3(position.x, 0.0f, position.y));

    TerrainVertexUniformBufferObject vertexUbo{};
    vertexUbo.modelMatrix = matrix;

    void* data;
    vkMapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory, 0, sizeof(vertexUbo), 0, &data);
    memcpy(data, &vertexUbo, sizeof(vertexUbo));
    vkUnmapMemory(renderer->device->device, vertexUniformBuffers[renderer->currentFrame]->bufferMemory);
}

void Terrain::CreateDesriptorSetLayout(VkDevice& device) {
    std::array<VkDescriptorSetLayoutBinding, 3> layoutBindings{};
    layoutBindings[0] = {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr};
    layoutBindings[1] = {1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
    layoutBindings[2] = {2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
    descriptorSetLayout = new DescriptorSetLayout(device, static_cast<uint32_t>(layoutBindings.size()), layoutBindings.data());
}

void Terrain::DeleteDesriptorSetLayout() {
    delete descriptorSetLayout;
}

void Terrain::createMesh(const std::string& heightMapFileName) {
    int width;
    int height;
    int channels;

    stbi_uc* pixels = stbi_load(heightMapFileName.c_str(), &width, &height, &channels, STBI_rgb);
    if (!pixels) {
        throw std::runtime_error("Failed to load texture!");
    }

    int vertexCount = height;

    std::vector<Vertex> vertices;
    vertices.resize(vertexCount * vertexCount);
	std::vector<uint32_t> indices;
    indices.resize(6 * (vertexCount - 1) * (vertexCount - 1));
	int vertexPointer = 0;
	for (int z = 0; z < vertexCount; z++) {
		for (int x = 0; x < vertexCount; x++) {
			vertices[vertexPointer].position = glm::vec3((float) x / ((float) vertexCount - 1) * SIZE, getHeight(x, z, width, height, pixels), (float) z / ((float) vertexCount - 1) * SIZE);
			vertices[vertexPointer].normal = getNormal(x, z, width, height, pixels);
			vertices[vertexPointer].textureCoordinates = glm::vec2((float) x / ((float) vertexCount - 1), 1.0f - ((float) z / ((float) vertexCount - 1)));
			vertexPointer++;
		}
	}
	int indexPointer = 0;
	for (int z = 0; z < vertexCount - 1; z++) {
		for (int x = 0; x < vertexCount - 1; x++) {
			int topLeft = ((z + 1) * vertexCount) + x;
			int topRight = topLeft + 1;
			int bottomLeft = (z * vertexCount) + x;
			int bottomRight = bottomLeft + 1;
			indices[indexPointer++] = bottomLeft;
			indices[indexPointer++] = topLeft;
			indices[indexPointer++] = topRight;
			indices[indexPointer++] = topRight;
			indices[indexPointer++] = bottomRight;
			indices[indexPointer++] = bottomLeft;
        }
	}

    mesh = new Mesh(vertices, indices, renderer);
}

float Terrain::getHeight(int x, int z, int width, int height, stbi_uc* heightMapPixels) {
    if (x < 0 || x >= width || z < 0 || z >= height) {
        return 0.0f;
    }

    width *= 3;
    x *= 3;
    unsigned char data = heightMapPixels[z * width + x];
    float h = data;
    h = 2.0f * (h / 255.0f) - 1.0f;
    h *= MAX_HEIGHT;
    h *= -1.0f;

    return h;
}

glm::vec3 Terrain::getNormal(int x, int z, int width, int height, stbi_uc* heightMapPixels) {
    float heightL = getHeight(x - 1, z, width, height, heightMapPixels);
    float heightR = getHeight(x + 1, z, width, height, heightMapPixels);
    float heightD = getHeight(z, z - 1, width, height, heightMapPixels);
    float heightU = getHeight(z, z + 1, width, height, heightMapPixels);
    glm::vec3 normal = glm::vec3(heightL - heightR, -2.0f, heightD - heightU);
    normal = glm::normalize(normal);

    return normal;
}
