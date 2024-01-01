#pragma once
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "runtime/resource/res_type/components/Camera.h"
#include <corecrt_math.h>
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
            return bilinearInterpolation(u, v);
        }


    private:
        glm::vec3 bilinearInterpolation(float u, float v) {
            const glm::vec3 cameraSpacePositon = glm::mix(glm::mix(m_bottomLeft, m_bottomRight, u),glm::mix(m_topLeft,m_topRight, u), v)*m_planeDistance;
            glm::vec3 worldSpacePosition = cameraSpacePositon + m_camera->cameraPosition;
            return worldSpacePosition;
        }
        std::shared_ptr<Camera> m_camera;
        glm::vec2 m_resolution;
        glm::vec3 m_topLeft,m_topRight,m_bottomLeft,m_bottomRight; 
        float m_planeDistance;
    };

}