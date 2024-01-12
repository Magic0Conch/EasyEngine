#include "Bone.h"
#include "glm/fwd.hpp"
#include "runtime/core/math/AssimpGLMHelpers.h"
#include <assimp/vector3.h>
#include <glm/gtx/quaternion.hpp>
using namespace EasyEngine;

Bone::Bone(const std::string& name,int ID,const aiNodeAnim* channel):m_name(name),m_id(ID),m_localTransform(1.0f){
    m_numPositions = channel->mNumPositionKeys;
    for (int positionIndex = 0; positionIndex<m_numPositions; ++positionIndex) {
        aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
        float timeStamp = channel->mPositionKeys[positionIndex].mTime;
        KeyPosition data;
        data.position = AssimpGLMHelpers::getGLMVec(aiPosition);
        data.timeStamp = timeStamp;
        m_positions.push_back(data);
    }
    m_numRotations = channel->mNumRotationKeys;
    for (int rotationIndex = 0; rotationIndex < m_numRotations; ++rotationIndex)
    {
        aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
        float timeStamp = channel->mRotationKeys[rotationIndex].mTime;
        KeyRotation data;
        data.orientation = AssimpGLMHelpers::getGLMQuat(aiOrientation);
        data.timeStamp = timeStamp;
        m_rotations.push_back(data);
    }

    m_numScalings = channel->mNumScalingKeys;
    for (int keyIndex = 0; keyIndex < m_numScalings; ++keyIndex)
    {
        aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
        float timeStamp = channel->mScalingKeys[keyIndex].mTime;
        KeyScale data;
        data.scale = AssimpGLMHelpers::getGLMVec(scale);
        data.timeStamp = timeStamp;
        m_scales.push_back(data);
    }

}

Bone::Bone(const std::string& name,int ID):m_name(name),m_id(ID),m_numPositions(0),m_numRotations(0),m_numScalings(0){

}

void Bone::addKeyPosition(const glm::vec3& position,float timeStamp){
    KeyPosition data;
    data.position = position;
    data.timeStamp = timeStamp;
    m_positions.push_back(data);
    ++m_numPositions;
}

void Bone::addKeyRotation(const glm::quat& rotation,float timeStamp){
    KeyRotation data;
    data.timeStamp = timeStamp;
    data.orientation = rotation;
    m_rotations.push_back(data);
    ++m_numRotations;
}

void Bone::addKeyScaling(const glm::vec3& scale,float timeStamp){
    KeyScale data;
    data.timeStamp = timeStamp;
    data.scale = scale;
    m_scales.push_back(data);
    ++m_numScalings;
}

void Bone::update(float animationTime,glm::mat4* translation,glm::mat4* rotation,glm::mat4* scale){
    glm::mat4* translation_ = translation,*rotation_ = rotation,*scale_ = scale;
    if(translation == nullptr)
        *translation_ = interpolatePosition(animationTime);
    if(rotation == nullptr){
        rotation_ = new glm::mat4();
        *rotation_ = interpolateRotation(animationTime);
    }
    if(scale == nullptr)
        *scale_ = interpolateScaling(animationTime);
    m_localTransform = (*translation_) * (*rotation_) * (*scale_);
}

void Bone::update(int frameIndex,glm::mat4* translation,glm::mat4* rotation,glm::mat4* scale){
    glm::mat4* translation_ = translation,*rotation_ = rotation,*scale_ = scale;
    if(translation == nullptr)
        *translation_ = interpolatePosition(frameIndex);
    if(rotation == nullptr){
        rotation_ = new glm::mat4();
        *rotation_ = interpolateRotation(frameIndex);
    }
    if(scale == nullptr)
        *scale_ = interpolateScaling(frameIndex);
    m_localTransform = (*translation_) * (*rotation_) * (*scale_);
    m_index = frameIndex% m_numRotations;
}


glm::mat4 Bone::getLocalTransform() {
    return m_localTransform; 
}
std::string Bone::getBoneName() const {
    return m_name; 
}

int Bone::getBoneID(){
    return m_id;
}

int Bone::seekPositionIndex(float animationTime){
    for(int index = 0;index < m_numPositions - 1;index++){
        if (animationTime < m_positions[index + 1].timeStamp)
            return index;
    }
    assert(0);
    return 0;
}

int Bone::seekRotationIndex(float animationTime){
    for (int index = 0; index < m_numRotations - 1; ++index)
    {
        if (animationTime < m_rotations[index + 1].timeStamp)
            return index;
    }
    assert(0);
    return 0;
}

int Bone::seekScaleIndex(float animationTime){
    for (int index = 0; index < m_numScalings - 1; ++index)
    {
        if (animationTime < m_scales[index + 1].timeStamp)
            return index;
    }
    assert(0);
    return 0;
}

float Bone::getScaleFactor(float lastTimeStamp,float nextTimeStamp,float animationTime){
    float scaleFactor = 0.0f;
    float midWayLength = animationTime - lastTimeStamp;
    float framesDiff = nextTimeStamp - lastTimeStamp;
    scaleFactor = midWayLength / framesDiff;
    return scaleFactor;
}

glm::mat4 Bone::interpolatePosition(float animationTime){
    if (1 == m_numPositions)
        return glm::translate(glm::mat4(1.0f), m_positions[0].position);

    int p0Index = seekPositionIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = getScaleFactor(m_positions[p0Index].timeStamp,
        m_positions[p1Index].timeStamp, animationTime);
    glm::vec3 finalPosition = glm::mix(m_positions[p0Index].position,
        m_positions[p1Index].position, scaleFactor);
    return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::interpolateRotation(float animationTime){
    if (1 == m_numRotations){
        auto rotation = glm::normalize(m_rotations[0].orientation);
        return glm::toMat4(rotation);
    }
    int p0Index = seekRotationIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = getScaleFactor(m_rotations[p0Index].timeStamp,
        m_rotations[p1Index].timeStamp, animationTime);
    glm::quat finalRotation = glm::slerp(m_rotations[p0Index].orientation,
        m_rotations[p1Index].orientation, scaleFactor);
    finalRotation = glm::normalize(finalRotation);
    return glm::toMat4(finalRotation);
}

glm::mat4 Bone::interpolateScaling(float animationTime){
    if (1 == m_numScalings)
        return glm::scale(glm::mat4(1.0f), m_scales[0].scale);

    int p0Index = seekScaleIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = getScaleFactor(m_scales[p0Index].timeStamp,
        m_scales[p1Index].timeStamp, animationTime);
    glm::vec3 finalScale = glm::mix(m_scales[p0Index].scale, m_scales[p1Index].scale
        , scaleFactor);
    return glm::scale(glm::mat4(1.0f), finalScale);
}

glm::mat4 Bone::interpolatePosition(int index){
    if (1 == m_numPositions)
        return glm::translate(glm::mat4(1.0f), m_positions[0].position);

    glm::quat finalRotation = m_rotations[m_index].orientation;
    finalRotation = glm::normalize(finalRotation);
}

glm::mat4 Bone::interpolateRotation(int index){
    if (1 == m_numRotations){
        auto rotation = glm::normalize(m_rotations[0].orientation);
        return glm::toMat4(rotation);
    }
    
    glm::quat finalRotation = m_rotations[m_index].orientation;
    finalRotation = glm::normalize(finalRotation);
    return glm::toMat4(finalRotation);
}

glm::mat4 Bone::interpolateScaling(int index){

    if (1 == m_numScalings)
        return glm::scale(glm::mat4(1.0f), m_scales[0].scale);

    glm::vec3 finalScale = m_scales[m_index].scale;
    return glm::scale(glm::mat4(1.0f), finalScale);
}