#include "Animator.h"
#include "runtime/core/math/Math.h"
#include "glm/fwd.hpp"
#include <cmath>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>
using namespace EasyEngine;

Animator::Animator(Animation* animation){
    m_currentTime = 0.0;
    m_currentAnimation = animation;
    m_finalBoneMatrices.reserve(100);
    m_frameIndex = 0;
    for (int i = 0; i<100; i++) {
        m_finalBoneMatrices.emplace_back(glm::mat4(1.0f));
    }
}

void Animator::updateAnimation(float dt){
    m_deltaTime = dt;
    if(m_currentAnimation){
        m_currentTime += m_currentAnimation->getTicksPerSecond() * dt;
        m_currentTime = fmod(m_currentTime,m_currentAnimation->getDuration());
        calculateBoneTransform(&m_currentAnimation->getRootNode(), glm::mat4(1.0));
        m_frameIndex = fmod(m_frameIndex+1,m_currentAnimation->getDuration());
    }
}

void Animator::playAnimation(Animation* animation){
    m_currentAnimation = animation;
    m_currentTime = 0.0f;
}

void Animator::calculateBoneTransform(const AssimpNodeData* node,glm::mat4 parentTransform){
    std::string nodeName = node->name;
    glm::mat4 nodeTransform = node->transformation;
    
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(nodeTransform, scale, rotation, translation, skew,perspective);
    rotation=glm::conjugate(rotation);
    glm::vec3 eurlerAngles = glm::eulerAngles(rotation);
    glm::vec3 eulerAnglesDegrees = glm::degrees(eurlerAngles);
    Bone* bone = m_currentAnimation->findBone(nodeName);

    glm::mat4 boneLocalTransform(1.0);
    if (bone)
    {
        glm::mat4 translationMat = Math::getTranformMatrix(nodeTransform);
        glm::mat4 scaleMat = glm::mat4(1.0);
        
        bone->update(m_frameIndex,&translationMat,nullptr,&scaleMat);
        if(nodeName=="thigh.L"||nodeName=="thigh.R"||nodeName=="middle.spine"){
            boneLocalTransform = Math::getRotationMatrix(bone->getLocalTransform());
        }
        else {
            nodeTransform = bone->getLocalTransform();
        }
    }

    glm::mat4 globalTransformation = parentTransform * nodeTransform * boneLocalTransform;

    auto boneInfoMap = m_currentAnimation->getBoneIDMap();
    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        int index = boneInfoMap[nodeName].id;
        glm::mat4 offset = boneInfoMap[nodeName].offset;
        m_finalBoneMatrices[index] = globalTransformation * offset;
    }

    for (int i = 0; i < node->childrenCount; i++)
        calculateBoneTransform(&node->children[i], globalTransformation);  
}

std::vector<glm::mat4> Animator::getFinalBoneMatrices(){
    return m_finalBoneMatrices;
}

