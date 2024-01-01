#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/gtx/quaternion.hpp"
#include "runtime/resource/res_type/components/Animation.h"
#include "runtime/resource/res_type/components/Animator.h"
#include "runtime/resource/res_type/components/Camera.h"
#include "runtime/resource/res_type/components/Model.h"
#include "../global/global_context.h"
#include "RenderPass.h"
namespace EasyEngine {
    using PU = EasyEngine::PathUtility;

    class SkeletalAnimationRenderPass:public RenderPass{
    public:
        SkeletalAnimationRenderPass(const string& shaderPath,Camera& camera);
        virtual void draw(Camera& camera);
        virtual void initialize();

        glm::quat calculateDeltaQuat(int frameIndex0,int frameIndex1,const std::string& name0,const std::string& name1){
            auto headPos0 = m_animationInputManager->getKeypoint(frameIndex0, name0);
            auto tailPos0 = m_animationInputManager->getKeypoint(frameIndex0, name1);

            auto headPos1 = m_animationInputManager->getKeypoint(frameIndex1, name0);
            auto tailPos1 = m_animationInputManager->getKeypoint(frameIndex1, name1);
            
            glm::vec3 dir0 = glm::normalize(tailPos0 - headPos0) ;
            glm::vec3 dir1 = glm::normalize(tailPos1- headPos1);

            glm::vec3 rotateAxis = glm::normalize(glm::cross(dir0,dir1));
            auto rotateAngle = glm::acos(glm::dot(dir0, dir1));
            // glm::mat4 rotatationMatrix = glm::rotate(glm::mat4(1.0), rotateAngle, rotateAxis);
            auto rotationQuaternion = glm::angleAxis(rotateAngle, rotateAxis);
            return rotationQuaternion;
        }

        glm::quat calculateDeltaQuat(int frameIndex,const std::string& head,const std::string& middle,const std::string& tail){
            auto headPosition = m_animationInputManager->getKeypoint(frameIndex, head);
            auto middlePosition = m_animationInputManager->getKeypoint(frameIndex, middle);
            auto tailPosition = m_animationInputManager->getKeypoint(frameIndex, tail);
            glm::vec3 directionTail = glm::normalize(tailPosition - middlePosition);
            glm::vec3 directionHead = glm::normalize(middlePosition - headPosition);
            // glm::mat4 rotationMatrix = glm::lookAt(glm::vec3(0), direction, glm::vec3(0.0f, 0.0f, -1.0f));

            glm::vec3 rotateAxis = glm::normalize(glm::cross(directionTail,directionHead));
            auto rotateAngle = glm::acos(glm::dot(directionHead, directionTail));
            // glm::mat4 rotatationMatrix = glm::rotate(glm::mat4(1.0), rotateAngle, rotateAxis);
            auto rotationQuaternion = glm::angleAxis(rotateAngle, rotateAxis);

            // auto rotationQuaternion = glm::toQuat(rotationMatrix);
            return rotationQuaternion;
        }

        void addBone();
        glm::quat calculateDeltaQuat(int frameIndex,const std::string& middle,const std::string& tail){
            
            auto middlePosition = m_animationInputManager->getKeypoint(frameIndex, middle);
            auto tailPosition = m_animationInputManager->getKeypoint(frameIndex, tail);
            glm::vec3 directionTail = glm::normalize(tailPosition - middlePosition);
            glm::vec3 directionHead = glm::vec3(1,0,0);
            // glm::mat4 rotationMatrix = glm::lookAt(glm::vec3(0), direction, glm::vec3(0.0f, 0.0f, -1.0f));

            glm::vec3 rotateAxis = glm::normalize(glm::cross(directionTail,directionHead));
            auto rotateAngle = glm::acos(glm::dot(directionHead, directionTail));
            // glm::mat4 rotatationMatrix = glm::rotate(glm::mat4(1.0), rotateAngle, rotateAxis);
            auto rotationQuaternion = glm::angleAxis(rotateAngle, rotateAxis);

            // auto rotationQuaternion = glm::toQuat(rotationMatrix);
            return rotationQuaternion;
        }
    private:
        std::shared_ptr<Animation> animation;
        std::shared_ptr<Animator> animator;
        std::shared_ptr<Model> model;
        Camera& m_camera;
        std::unique_ptr<AnimationInputManager> m_animationInputManager;
    };
}