#ifndef animator_hpp
#define animator_hpp

#include <array>

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
    std::array<Keyframe> getPreviousAndNextFrames();
    float calculateProgression(Keyframe& previousFrame, Keyframe& nextFrame);
    std::unordered_map<std::string, glm::mat4> interpolatePoses(Keyframe& x, Keyframe& y, float a);
};

#endif
