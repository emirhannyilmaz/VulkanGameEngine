#ifndef terrain_mesh_hpp
#define terrain_mesh_hpp

#include <vector>
#include "terrain_vertex.hpp"
#include "../renderer/buffer.hpp"

class Renderer;

class TerrainMesh {
public:
    Buffer* vertexBuffer;
    Buffer* indexBuffer;
    size_t indicesSize;
    TerrainMesh(std::vector<TerrainVertex>& vertices, std::vector<uint32_t>& indices, Renderer* renderer);
    ~TerrainMesh();
};

#endif
