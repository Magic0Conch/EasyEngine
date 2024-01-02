#pragma once
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
namespace EasyEngine {
    class Math{
    public:
        Math();
        
        template<class T>
        static T lerp(T a,T b,T f){
            return a + f*(b-a);
        }



        static glm::mat4 getTranformMatrix(const glm::mat4& transformMatrix){
            return glm::translate(glm::mat4(1.0f),  glm::vec3(transformMatrix[3]));
        }

        template<class T>
        static T bilinearInterpolation(float u, float v,const T& m_bottomLeft,const T& m_bottomRight,const T& m_topLeft,const T& m_topRight) {
            const T result = glm::mix(glm::mix(m_bottomLeft, m_bottomRight, u),glm::mix(m_topLeft,m_topRight, u), v);
            return result;
        }
    };

}