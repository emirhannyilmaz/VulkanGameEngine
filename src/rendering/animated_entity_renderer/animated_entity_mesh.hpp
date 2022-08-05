#ifndef animated_entity_mesh_hpp
#define animated_entity_mesh_hpp

#include <vector>
#include "animated_entity_vertex.hpp"
#include "../renderer/renderer.hpp"
#include "../renderer/buffer.hpp"

class AnimatedEntityMesh {
public:
    Buffer* vertexBuffer;
    Buffer* indexBuffer;
    size_t indicesSize;
    AnimatedEntityMesh(std::vector<AnimatedEntityVertex> vertices, std::vector<uint32_t> indices, Renderer* renderer);
    ~AnimatedEntityMesh();
};

#endif
