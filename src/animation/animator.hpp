#ifndef animator_hpp
#define animator_hpp

#include "../entities/animated_entity.hpp"
#include "animation.hpp"
#include <unordered_map>
#include <array>
#include <string>
#include <glm/glm.hpp>
#include "keyframe.hpp"

class Animator {
public:
    Animator(AnimatedEntity* entity);
    void setAnimation(Animation* animation);
    void update(float speed, float deltaTime);
private:
    AnimatedEntity* entity;
    Animation* animation = nullptr;
    float time = 0.0f;
    void increaseTime(float speed, float deltaTime);
    std::unordered_map<std::string, glm::mat4> calculateCurrentPose();
    void applyPoseToJoints(std::unordered_map<std::string, glm::mat4>& currentPose, Joint* joint, glm::mat4 parentTransform);
    std::array<Keyframe, 2> getPreviousAndNextFrames();
    float calculateProgression(Keyframe& previousFrame, Keyframe& nextFrame);
    std::unordered_map<std::string, glm::mat4> interpolatePoses(Keyframe& x, Keyframe& y, float a);
};

#endif
