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
    Mesh(std::vector<Vertex> vertices, std::vector<uint16_t> indices, Renderer* renderer);
    ~Mesh();
};

#endif
