#ifndef animator_hpp
#define animator_hpp

#include "../entities/animated_entity.hpp"
#include "../rendering/renderer/renderer.hpp"
#include "animation.hpp"
#include <unordered_map>
#include <array>
#include <string>
#include <glm/glm.hpp>
#include "keyframe.hpp"

class Animator {
public:
    Animator(AnimatedEntity* entity, Renderer* renderer);
    void setAnimation(Animation* animation);
    void update();
private:
    AnimatedEntity* entity;
    Renderer* renderer;
    Animation* animation = nullptr;
    float time = 0.0f;
    void increaseTime();
    std::unordered_map<std::string, glm::mat4> calculateCurrentPose();
    void applyPoseToJoints(std::unordered_map<std::string, glm::mat4>& currentPose, Joint* joint, glm::mat4 parentTransform);
    std::array<Keyframe, 2> getPreviousAndNextFrames();
    float calculateProgression(Keyframe& previousFrame, Keyframe& nextFrame);
    std::unordered_map<std::string, glm::mat4> interpolatePoses(Keyframe& x, Keyframe& y, float a);
};

#endif
