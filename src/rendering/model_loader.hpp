#ifndef model_loader_hpp
#define model_loader_hpp

#include "vertex.hpp"
#include <vector>
#include <unordered_map>

struct ObjModelData {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};

class ModelLoader {
public:
    static ObjModelData LoadObj(const char* filePath);
};

#endif
