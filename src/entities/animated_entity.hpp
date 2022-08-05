#ifndef animated_entity_hpp
#define animated_entity_hpp

#include "../rendering/animated_entity_renderer/animated_entity_mesh.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../rendering/renderer/descriptor_pool.hpp"
#include "../rendering/renderer/descriptor_set_layout.hpp"
#include "../rendering/renderer/descriptor_sets.hpp"
#include "../rendering/renderer/renderer_info.hpp"
#include "joint.hpp"

class Texture;

struct AnimatedEntityVertexUniformBufferObject {
    alignas(16) glm::mat4 modelMatrix;
    alignas(16) glm::mat4 jointTransforms[50];
};

struct AnimatedEntityFragmentUniformBufferObject {
    alignas(4) float reflectivity;
    alignas(4) float shineDamper;
};

class AnimatedEntity {
public:
    AnimatedEntityMesh* mesh;
    Texture* texture;
    Joint* rootJoint;
    int jointCount;
    static DescriptorSetLayout* descriptorSetLayout;
    DescriptorSets* descriptorSets;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    AnimatedEntity(AnimatedEntityMesh* mesh, Texture* texture, Joint* rootJoint, int jointCount, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Renderer* renderer);
    ~AnimatedEntity();
    std::vector<glm::mat4> getJointTransforms();
    void addJointTransformToVector(Joint* parentJoint, std::vector<glm::mat4>& jointTransforms);
    void updateDescriptorSetResources();
    static void CreateDesriptorSetLayout(VkDevice& device);
    static void DeleteDesriptorSetLayout();
private:
    DescriptorPool* descriptorPool;
    Renderer* renderer;
    std::vector<Buffer*> vertexUniformBuffers;
    std::vector<Buffer*> fragmentUniformBuffers;
};

#endif
