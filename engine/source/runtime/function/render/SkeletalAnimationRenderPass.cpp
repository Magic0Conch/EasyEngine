#include "SkeletalAnimationRenderPass.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "runtime/resource/res_type/components/Animation.h"
#include "runtime/resource/res_type/components/Animator.h"
#include "runtime/resource/res_type/components/Camera.h"
#include "runtime/resource/res_type/components/Model.h"
#include "runtime/function/input/AnimationInputManager.h"
#include "runtime/include/WindowTime.h"
#include <memory>
#include <string>
#include <vector>


using namespace EasyEngine;
//To get visuals similar to Phong shading the specular shininess exponent has to be set a bit higher. 
//A general rule of thumb is to set it between 2 and 4 times the Phong shininess exponent. 
SkeletalAnimationRenderPass::SkeletalAnimationRenderPass(const string& shaderPath,Camera& camera)
: RenderPass(shaderPath),
m_camera(camera){
    m_pointShader = std::make_shared<Shader>(PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "common/point.vert").c_str()
               ,PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "common/point.frag").c_str());
    uniformBlockIndex = glGetUniformBlockIndex(m_pointShader->ID,"Matrices");
    glUniformBlockBinding(m_pointShader->ID,uniformBlockIndex,0);
}

void pushVec3Point(std::vector<GLfloat>& container,const glm::vec3& point){
    container.emplace_back(point.x);
    container.emplace_back(point.y);
    container.emplace_back(point.z);
}

void modifyVec3Point(std::vector<GLfloat>& container,const glm::vec3& point,int index){
    container[index*3] = (point.x);
    container[index*3+1] = (point.y);
    container[index*3+2] = (point.z);
}


void SkeletalAnimationRenderPass::initialize(){
    const auto modelPath = PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),"character/ske.fbx");
    const auto animationPath = PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),"character/ske.fbx");
    this->model = make_shared<Model>(modelPath);
    this->animation = make_shared<Animation>(animationPath,this->model.get());
    this->animator = make_shared<Animator>(this->animation.get());
    this->animation->setDuration(400);
    m_animationInputManager = make_unique<AnimationInputManager>(m_camera,glm::vec2(1920,1080),4.2);
    m_animationInputManager->readPoseFromJson("pose/results_test1.json");
    m_animationInputManager->readBoneLengthFromJson("pose/jiangling.json");
    m_animationInputManager->readBoneDirectionFromJson("pose/T_pose_init.json");
    m_animationInputManager->screenPosition2WorldPosition();
    addBone();
    //debug
    pushVec3Point(m_points, m_animationInputManager->getKeypoint(0, "middle_shoulder"));
    pushVec3Point(m_points, m_animationInputManager->getKeypoint(0, "middle_hip"));
    pushVec3Point(m_points, m_animationInputManager->getKeypoint(0, "right_shoulder"));
    pushVec3Point(m_points, m_animationInputManager->getKeypoint(0, "left_shoulder"));
    pushVec3Point(m_points, m_animationInputManager->getKeypoint(0, "left_hip"));
    pushVec3Point(m_points, m_animationInputManager->getKeypoint(0, "right_hip"));
    pushVec3Point(m_points, m_animationInputManager->getKeypoint(0, "left_elbow"));
    pushVec3Point(m_points, m_animationInputManager->getKeypoint(0, "right_elbow"));
    pushVec3Point(m_points, m_animationInputManager->getKeypoint(0, "left_knee"));
    pushVec3Point(m_points, m_animationInputManager->getKeypoint(0, "right_knee"));
    pushVec3Point(m_points, m_animationInputManager->getKeypoint(0, "left_wrist"));
    pushVec3Point(m_points, m_animationInputManager->getKeypoint(0, "right_wrist"));
    pushVec3Point(m_points, m_animationInputManager->getKeypoint(0, "left_ankle"));
    pushVec3Point(m_points, m_animationInputManager->getKeypoint(0, "right_ankle"));
    pushVec3Point(m_points, m_animationInputManager->getKeypoint(0, "nose"));
    
    
    glGenVertexArrays(1, &m_pointVAO);
    glGenBuffers(1, &m_pointVBO);

    glBindVertexArray(m_pointVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_pointVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_points.size(), m_points.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}




void SkeletalAnimationRenderPass::addBone(){
    auto boneInfoMap = animation->getBoneIDMap();
    
    const int frameCount = m_animationInputManager->getFrameCount();
    const int fps = 1;
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
    Bone* boneMiddleSpine = new Bone("middle.spine",boneInfoMap["middle.spine"].id);


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
    boneMiddleSpine->addKeyRotation(glm::quat(1,0,0,0), 0);
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

        auto upperLegLeft = calculateDeltaByInitPose(frameIndex, "left_hip","left_knee", "thigh.L");
        // auto upperLegLeft = calculateDeltaQuat(frameIndex,"hip_L_up","left_hip","left_knee");
        boneUpperLegLeft->addKeyRotation(upperLegLeft, frameIndex);
        auto upperLegRight = calculateDeltaByInitPose(frameIndex, "right_hip","right_knee", "thigh.R");
        // auto upperLegRight = calculateDeltaQuat(frameIndex,"hip_R_up","right_hip","right_knee");
        boneUpperLegRight->addKeyRotation(upperLegRight, frameIndex);
        
        auto neck = calculateDeltaQuat(frameIndex,"middle_spine","neck","nose");
        boneNeck->addKeyRotation(neck, frameIndex);

        auto middleSpine = calculateDeltaByInitPose(frameIndex, "middle_hip","middle_shoulder", "middle.spine");
        boneMiddleSpine->addKeyRotation(middleSpine, frameIndex);
        
        // boneMiddleSpine
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
    this->animation->pushBone(*boneMiddleSpine);
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
    this->model->draw(*shader);

    //debug
    m_pointShader->use();
    glm::mat4 debugPointModel(1.0);
    if(frameCnt>0){
        modifyVec3Point(m_points,m_animationInputManager->getKeypoint(frameCnt, "middle_shoulder"),0);
        modifyVec3Point(m_points,m_animationInputManager->getKeypoint(frameCnt, "middle_hip"),1);
        modifyVec3Point(m_points,m_animationInputManager->getKeypoint(frameCnt, "right_shoulder"),2);
        modifyVec3Point(m_points,m_animationInputManager->getKeypoint(frameCnt, "left_shoulder"),3);
        modifyVec3Point(m_points,m_animationInputManager->getKeypoint(frameCnt, "left_hip"),4);
        modifyVec3Point(m_points,m_animationInputManager->getKeypoint(frameCnt, "right_hip"),5);
        modifyVec3Point(m_points,m_animationInputManager->getKeypoint(frameCnt, "left_elbow"),6);
        modifyVec3Point(m_points,m_animationInputManager->getKeypoint(frameCnt, "right_elbow"),7);
        modifyVec3Point(m_points,m_animationInputManager->getKeypoint(frameCnt, "left_knee"),8);
        modifyVec3Point(m_points,m_animationInputManager->getKeypoint(frameCnt, "right_knee"),9);
        modifyVec3Point(m_points,m_animationInputManager->getKeypoint(frameCnt, "left_wrist"),10);
        modifyVec3Point(m_points,m_animationInputManager->getKeypoint(frameCnt, "right_wrist"),11);
        modifyVec3Point(m_points,m_animationInputManager->getKeypoint(frameCnt, "left_ankle"),12);
        modifyVec3Point(m_points,m_animationInputManager->getKeypoint(frameCnt, "right_ankle"),13);
        modifyVec3Point(m_points,m_animationInputManager->getKeypoint(frameCnt, "nose"),14);

    }
    
    ++frameCnt;
    frameCnt = frameCnt % (int)this->animation->getDuration();
    debugPointModel = glm::translate(debugPointModel, glm::vec3(-1,0,0));
    m_pointShader->setValue("model",debugPointModel);
    glBindVertexArray(m_pointVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_pointVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_points.size(), m_points.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(GL_POINTS, 0, m_points.size()/3);
    glBindVertexArray(0);


}
