#include "Animation.h"
#include "runtime/resource/res_type/components/Animation.h"
#include "runtime/core/math/AssimpGLMHelpers.h"
#include <algorithm>

using namespace EasyEngine;

Animation::Animation(const std::string& animationPath, Model* model){
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
    assert(scene && scene->mRootNode);
    auto animation = scene->mAnimations[0];
    m_duration = animation->mDuration;
    m_ticksPerSecond = animation->mTicksPerSecond;
    readHeirarchyData(m_rootNode, scene->mRootNode);
    readMissingBones(animation, *model);
}

Animation::~Animation(){

}

Bone* Animation::findBone(const std::string& name){
    auto iter = std::find_if(m_bones.begin(),m_bones.end(),[&](const Bone& bone){
        return bone.getBoneName() == name;
    });
    if(iter == m_bones.end()) 
        return nullptr;
    else
        return &(*iter);
}

void Animation::readMissingBones(const aiAnimation* animation, Model& model){
    int size = animation->mNumChannels;
    auto& boneInfoMap = model.getBoneInfoMap();
    int& boneCount = model.getBoneCount();

    //reading channels(bones engaged in an animation and their keyframes)
    for (int i = 0; i < size; i++)
    {
        auto channel = animation->mChannels[i];
        std::string boneName = channel->mNodeName.data;

        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            boneInfoMap[boneName].id = boneCount;
            boneCount++;
        }
        m_bones.push_back(Bone(channel->mNodeName.data,
            boneInfoMap[channel->mNodeName.data].id, channel));
    }
    m_boneInfoMap = boneInfoMap;
}

void Animation::readHeirarchyData(AssimpNodeData& dest, const aiNode* src){
    assert(src);
    
    dest.name = src->mName.data;
    dest.transformation = AssimpGLMHelpers::convertMatrixToGLMFormat(src->mTransformation);
    dest.childrenCount = src->mNumChildren;

    for (int i = 0; i < src->mNumChildren; i++){
        AssimpNodeData newData;
        readHeirarchyData(newData, src->mChildren[i]);
        dest.children.push_back(newData);
    }
}
