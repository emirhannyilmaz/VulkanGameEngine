#ifndef entity_mesh_hpp
#define entity_mesh_hpp

#include <vector>
#include "entity_vertex.hpp"
#include "../renderer/buffer.hpp"

class Renderer;

class EntityMesh {
public:
    Buffer* vertexBuffer;
    Buffer* indexBuffer;
    size_t indicesSize;
    EntityMesh(std::vector<EntityVertex>& vertices, std::vector<uint32_t>& indices, Renderer* renderer);
    ~EntityMesh();
};

#endif
