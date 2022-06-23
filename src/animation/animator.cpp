#include "animator.hpp"

Animator::Animator(AnimatedEntity* entity, Renderer* renderer) {
    this->entity = entity;
    this->renderer = renderer;
}

void Animator::setAnimation(Animation* animation) {
    time = 0.0f;
    this->animation = animation;
}

void Animator::update() {
    if (animation == nullptr) {
        return;
    }

    increaseTime();
    std::unordered_map<std::string, glm::mat4> currentPose = calculateCurrentPose();
}

void Animator::increaseTime() {
    time += (float) renderer->deltaTime;
    if (time > animation->length) {
        time = fmod(time, animation->length);
    }
}

std::unordered_map<std::string, glm::mat4> Animator::calculateCurrentPose() {
    std::array<Keyframe> frames = getPreviousAndNextFrames();
    float progression = calculateProgression(frames[0], frames[1]);

    return interpolatePoses(frames[0], frames[1], progression);
}

std::array<Keyframe> Animator::getPreviousAndNextFrames() {
    std::vector<Keyframe> keyframes = animation->keyframes;
    Keyframe previousFrame = keyframes[0];
    Keyframe nextFrame = keyframes[0];
    for (int i = 1; i < keyframes.size(); i++) {
        nextFrame = keyframes[i];
        if (nextFrame.timestamp > time) {
            break;
        }
        previousFrame = keyframes[i];
    }

    return {previousFrame, nextFrame};
}

float Animator::calculateProgression(Keyframe& previousFrame, Keyframe& nextFrame) {
    float totalTime = nextFrame.timestamp - previousFrame.timestamp;
    float currentTime = time - previousFrame.timestamp;

    return currentTime / totalTime;
}

std::unordered_map<std::string, glm::mat4> Animator::interpolatePoses(Keyframe& x, Keyframe& y, float a) {
    std::unordered_map<std::string, glm::mat4> pose;
    for (auto joint : x.pose) {
        JointTransform previousJointTransform = joint.second;
        JointTransform nextJointTransform = y.pose[joint.first];
        JointTransform newJointTransform = JointTransform::Interpolate(previousJointTransform, nextJointTransform, a);
        pose.insert({joint.first, newJointTransform.createTransformationMatrix()});
    }

    return pose;
}