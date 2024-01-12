#pragma once
#include "glm/exponential.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "runtime/core/math/Math.h"
#include "runtime/resource/res_type/components/Camera.h"
#include "Math.h"
#include <cmath>
#include <corecrt_math.h>
#include <cstdlib>
#include <memory>
#include <mutex>
namespace EasyEngine {
    class CoordinateMapper{
    public:
        CoordinateMapper(Camera& camera,const glm::vec2& resolution,float planeDistance):
        m_camera(std::make_shared<Camera>(camera)),
        m_resolution(resolution),
        m_planeDistance(planeDistance){
            calculateCorners();
        }

        void calculateCorners(){
            using namespace glm;
            const auto& fov = m_camera->fov;
            const auto& aspect = m_resolution.x/m_resolution.y;
            const auto& cameraUpDirection = glm::normalize(m_camera->cameraUp);
            const auto& cameraRightDirection = glm::normalize(m_camera->cameraRight);
            const auto& cameraForwardDirection = glm::normalize(m_camera->cameraFront);
            const float halfHeight = tan(glm::radians(fov*0.5f));
            const float halfWidth = halfHeight*aspect;

            const vec3 toTop = cameraUpDirection * halfHeight;
            const vec3 toRight = cameraRightDirection * halfWidth;
            
            m_topLeft = cameraForwardDirection + toTop - toRight;
            float scale = glm::length(m_topLeft);
            m_topLeft = normalize(m_topLeft);
            m_topLeft*=scale;

            m_topRight = cameraForwardDirection + toTop + toRight;
            m_topRight = normalize(m_topRight);
            m_topRight*=scale;

            m_bottomLeft = cameraForwardDirection - toTop - toRight;
            m_bottomLeft = normalize(m_bottomLeft);
            m_bottomLeft*=scale;

            m_bottomRight = cameraForwardDirection - toTop + toRight;
            m_bottomRight = normalize(m_bottomRight);
            m_bottomRight*=scale;

        }

        glm::vec3 screen2world(float screenSpaceX,float screenSpaceY){
            const float u = screenSpaceX/m_resolution.x;
            const float v = 1-screenSpaceY/m_resolution.y;
            return getWorldSpacePositon(u, v);
        }

        glm::vec3 screen2WorldFromRefPos(const glm::vec3& refWorldPosition,const glm::vec2& screenPos,const float boneLength){
            const float distanceBetweenCameraAndRefWorldPosition = glm::distance(refWorldPosition, this->m_camera->cameraPosition);
            const float u = screenPos.x/m_resolution.x;
            const float v = 1-screenPos.y/m_resolution.y;
            auto dir = Math::bilinearInterpolation(u, v, m_bottomLeft, m_bottomRight, m_topLeft, m_topRight);
            dir = glm::normalize(dir);
            auto cameraToRefDir = glm::normalize(glm::vec3(refWorldPosition - this->m_camera->cameraPosition));
            const float cosTheta = glm::dot(cameraToRefDir, dir);

            double aSuqare = distanceBetweenCameraAndRefWorldPosition * distanceBetweenCameraAndRefWorldPosition;
            double discriminantSquare = abs(4*aSuqare*cosTheta*cosTheta - 4*(aSuqare-boneLength*boneLength));
            double discriminant = sqrt(discriminantSquare);
            float distanceBetweenCameraAndTarget = (2*distanceBetweenCameraAndRefWorldPosition*cosTheta+discriminant)/2;
            // if(discriminantSquare<0) 
            //     distanceBetweenCameraAndTarget = distanceBetweenCameraAndRefWorldPosition;
            auto cameraSpacePos = dir*distanceBetweenCameraAndTarget;
            auto worldPos = cameraSpacePos + this->m_camera->cameraPosition;
            return worldPos;
        }

    private:
        glm::vec3 getCameraSpacePosition(float u,float v){
            const glm::vec3 cameraSpacePositon =  Math::bilinearInterpolation(u, v, m_bottomLeft, m_bottomRight, m_topLeft, m_topRight) *m_planeDistance;
            return cameraSpacePositon;
        }

        glm::vec3 getWorldSpacePositon(float u,float v){
            const auto cameraSpacePositon = getCameraSpacePosition(u, v);
            auto worldSpacePosition = cameraSpacePositon + m_camera->cameraPosition;
            return worldSpacePosition;
        }

        // glm::vec3 bilinearInterpolation(float u, float v) {
        //     const glm::vec3 cameraSpacePositon =  Math::bilinearInterpolation(u, v, m_bottomLeft, m_bottomRight, m_topLeft, m_topRight) *m_planeDistance;
        //     glm::vec3 worldSpacePosition = cameraSpacePositon + m_camera->cameraPosition;
        //     return worldSpacePosition;
        // }
        std::shared_ptr<Camera> m_camera;
        glm::vec2 m_resolution;
        glm::vec3 m_topLeft,m_topRight,m_bottomLeft,m_bottomRight; 
        float m_planeDistance;
    };

}