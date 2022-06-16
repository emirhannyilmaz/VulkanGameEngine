#ifndef animated_mesh_hpp
#define animated_mesh_hpp

#include <vector>
#include "animated_vertex.hpp"
#include "../renderer/renderer.hpp"

class AnimatedMesh {
public:
    Buffer* vertexBuffer;
    Buffer* indexBuffer;
    size_t indicesSize;
    AnimatedMesh(std::vector<AnimatedVertex> vertices, std::vector<uint32_t> indices, Renderer* renderer);
    ~AnimatedMesh();
};

#endif
