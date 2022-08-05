#ifndef water_mesh_hpp
#define water_mesh_hpp

#include <vector>
#include "water_vertex.hpp"
#include "../renderer/buffer.hpp"

class Renderer;

class WaterMesh {
public:
    Buffer* vertexBuffer;
    Buffer* indexBuffer;
    size_t indicesSize;
    WaterMesh(std::vector<WaterVertex>& vertices, std::vector<uint32_t>& indices, Renderer* renderer);
    ~WaterMesh();
};

#endif
