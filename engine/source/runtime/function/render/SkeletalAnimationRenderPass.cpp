#include "SkeletalAnimationRenderPass.h"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/geometric.hpp"
#include "runtime/resource/res_type/components/Animation.h"
#include "runtime/resource/res_type/components/Animator.h"
#include "runtime/resource/res_type/components/Camera.h"
#include "runtime/resource/res_type/components/Model.h"
#include "runtime/function/input/AnimationInputManager.h"
#include "runtime/include/WindowTime.h"
#include <memory>
#include <string>


using namespace EasyEngine;
//To get visuals similar to Phong shading the specular shininess exponent has to be set a bit higher. 
//A general rule of thumb is to set it between 2 and 4 times the Phong shininess exponent. 
SkeletalAnimationRenderPass::SkeletalAnimationRenderPass(const string& shaderPath,Camera& camera)
: RenderPass(shaderPath),
m_camera(camera){}

void SkeletalAnimationRenderPass::initialize(){
    const auto modelPath = PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),"character/ske.fbx");
    const auto animationPath = PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),"character/ske.fbx");
    this->model = make_shared<Model>(modelPath);
    this->animation = make_shared<Animation>(animationPath,this->model.get());
    this->animator = make_shared<Animator>(this->animation.get());
    this->animation->setDuration(525);
    m_animationInputManager = make_unique<AnimationInputManager>(m_camera,glm::vec2(640,480),2.4);
    m_animationInputManager->readPoseFromJson("pose/results_test_pose.json");
    addBone();
}

void SkeletalAnimationRenderPass::addBone(){
    auto boneInfoMap = animation->getBoneIDMap();
    
    const int frameCount = m_animationInputManager->getFrameCount();
    const int fps = 25;
    int frameIndex = 0;
    Bone* boneForeArmLeft = new Bone("forearm.L",boneInfoMap["forearm.L"].id);
    Bone* boneForeArmRight = new Bone("forearm.R",boneInfoMap["forearm.R"].id);
    Bone* boneUpperArmLeft = new Bone("upper_arm.L",boneInfoMap["upper_arm.L"].id);
    Bone* boneUpperArmRight = new Bone("upper_arm.R",boneInfoMap["upper_arm.R"].id);

    Bone* boneForeLegLeft = new Bone("shin.L",boneInfoMap["shin.L"].id);
    Bone* boneForeLegRight = new Bone("shin.R",boneInfoMap["shin.R"].id);
    Bone* boneUpperLegLeft = new Bone("thigh.L",boneInfoMap["thigh.L"].id);
    Bone* boneUpperLegRight = new Bone("thigh.R",boneInfoMap["thigh.R"].id);
    
    Bone* boneNeck = new Bone("neck",boneInfoMap["neck"].id);


    // Bone* boneForeArmLeft = new Bone("forearm.R",boneInfoMap["forearm.R"].id);
    boneForeArmLeft->addKeyRotation(glm::quat(1,0,0,0), 0);
    boneForeArmRight->addKeyRotation(glm::quat(1,0,0,0), 0);
    boneUpperArmLeft->addKeyRotation(glm::quat(1,0,0,0), 0);
    boneUpperArmRight->addKeyRotation(glm::quat(1,0,0,0), 0);
    boneForeLegLeft->addKeyRotation(glm::quat(1,0,0,0), 0);
    boneForeLegRight->addKeyRotation(glm::quat(1,0,0,0), 0);
    boneUpperLegLeft->addKeyRotation(glm::quat(1,0,0,0), 0);
    boneUpperLegRight->addKeyRotation(glm::quat(1,0,0,0), 0);
    boneNeck->addKeyRotation(glm::quat(1,0,0,0), 0);
    while (frameIndex<frameCount) {
        // calculateDeltaQuat
        auto leftElbow = calculateDeltaQuat(frameIndex,"left_shoulder", "left_elbow", "left_wrist");
        boneForeArmLeft->addKeyRotation(leftElbow, frameIndex);
        auto rightElbow = calculateDeltaQuat(frameIndex,"right_shoulder", "right_elbow", "right_wrist");
        boneForeArmRight->addKeyRotation(rightElbow, frameIndex);
        auto leftForeArm = calculateDeltaQuat(frameIndex,"middle_spine", "left_shoulder", "left_elbow");
        boneUpperArmLeft->addKeyRotation(leftForeArm, frameIndex);
        auto rightForeArm = calculateDeltaQuat(frameIndex,"middle_spine","right_shoulder","right_elbow");
        boneUpperArmRight->addKeyRotation(rightForeArm, frameIndex);
        
        auto foreLegLeft = calculateDeltaQuat(frameIndex,"left_hip","left_knee","left_ankle");
        boneForeLegLeft->addKeyRotation(foreLegLeft, frameIndex);
        auto foreLegRight = calculateDeltaQuat(frameIndex,"right_hip","right_knee","right_ankle");
        boneForeLegRight->addKeyRotation(foreLegRight, frameIndex);
        auto upperLegLeft = calculateDeltaQuat(frameIndex,"lower_spine","left_hip","left_knee");
        boneUpperLegLeft->addKeyRotation(upperLegLeft, frameIndex);
        auto upperLegRight = calculateDeltaQuat(frameIndex,"lower_spine","right_hip","right_knee");
        boneUpperLegRight->addKeyRotation(upperLegRight, frameIndex);
        
        auto neck = calculateDeltaQuat(frameIndex,"middle_spine","neck","nose");
        boneNeck->addKeyRotation(neck, frameIndex);
        frameIndex+=fps;
    }
    this->animation->pushBone(*boneForeArmLeft);
    this->animation->pushBone(*boneForeArmRight);
    this->animation->pushBone(*boneUpperArmLeft);
    this->animation->pushBone(*boneUpperArmRight);
    this->animation->pushBone(*boneForeLegLeft);
    this->animation->pushBone(*boneForeLegRight);
    this->animation->pushBone(*boneUpperLegLeft);
    this->animation->pushBone(*boneUpperLegRight);
    this->animation->pushBone(*boneNeck);
}

void SkeletalAnimationRenderPass::draw(Camera& camera){
    auto dt = WindowTime::deltaTimeValue;
    animator->updateAnimation(dt);
    shader->use();
    auto transforms = animator->getFinalBoneMatrices();
    for (int i = 0; i < transforms.size(); i++) {
        shader->setValue("finalBonesMatrices["+to_string(i)+"]",transforms[i]);
    }
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
    model = glm::scale(model, glm::vec3(.01f, .01f, .01f));	
    shader->setValue("model",model);
    // shader->setValue("model",model);
    this->model->draw(*shader);
}
