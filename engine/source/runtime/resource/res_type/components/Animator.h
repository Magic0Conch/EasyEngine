#pragma once
#include "glm/fwd.hpp"
#include "runtime/resource/res_type/components/Animation.h"
#include <vector>
namespace EasyEngine {
class Animator{
public:
    Animator(Animation* animation);
    void updateAnimation(float dt);
    void playAnimation(Animation* animation);
    void calculateBoneTransform(const AssimpNodeData* node,glm::mat4 parentTransform);
    std::vector<glm::mat4> getFinalBoneMatrices();
private:
    std::vector<glm::mat4> m_finalBoneMatrices;
    Animation* m_currentAnimation;
    float m_currentTime;
    float m_deltaTime;
};
}