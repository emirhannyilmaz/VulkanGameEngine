#ifndef collada_loader_hpp
#define collada_loader_hpp

#include "../rendering/entity_renderer/vertex.hpp"
#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <fstream>

struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};

class ColladaLoader {
public:
    static MeshData LoadMesh(const std::string& fileName);
};

#endif
