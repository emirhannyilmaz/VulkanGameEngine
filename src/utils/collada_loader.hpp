#ifndef collada_loader_hpp
#define collada_loader_hpp

#include "../rendering/entity_renderer/entity_vertex.hpp"
#include "../rendering/animated_entity_renderer/animated_entity_vertex.hpp"
#include "../entities/joint.hpp"
#include "../animation/animation.hpp"
#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

struct MeshData {
    std::vector<EntityVertex> vertices;
    std::vector<uint32_t> indices;
};

struct AnimatedMeshData {
    std::vector<AnimatedEntityVertex> vertices;
    std::vector<uint32_t> indices;
};

struct AnimationData {
    Animation animation;
    Joint* rootJoint;
    int jointCount;
};

class ColladaLoader {
public:
    static MeshData LoadMesh(const std::string& fileName);
    static AnimatedMeshData LoadAnimatedMesh(const std::string& fileName);
    static AnimationData LoadAnimation(const std::string& fileName);
private:
    static void AddJointToChildren(Joint* rootJoint, std::string parentJointName, Joint* newJoint);
};

#endif
