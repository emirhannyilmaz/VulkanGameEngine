#ifndef mesh_hpp
#define mesh_hpp

#include <vector>
#include "vertex.hpp"
#include "buffer.hpp"

class Renderer;

class Mesh {
public:
    Buffer* vertexBuffer;
    Buffer* indexBuffer;
    size_t indicesSize;
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, Renderer* renderer);
    ~Mesh();
};

#endif
