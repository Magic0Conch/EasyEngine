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

        static void decomposeMatrix(const glm::mat4& transformMatrix, glm::mat4* translation, glm::mat4* rotation, glm::mat4* scale){
            if(translation!=nullptr)
                *translation = glm::translate(glm::mat4(1.0f),  glm::vec3(transformMatrix[3]));
            float x = glm::length(glm::vec3(transformMatrix[0]));
            float y = glm::length(glm::vec3(transformMatrix[1]));
            float z = glm::length(glm::vec3(transformMatrix[2]));
            if(scale!=nullptr){
                scale = new glm::mat4(1.0);
                (*scale)[0][0] = x;
                (*scale)[1][1] = y;
                (*scale)[2][2] = z;
            }
            if(rotation!=nullptr){
                rotation = new glm::mat4(transformMatrix);
                rotation[0]/=x;
                rotation[1]/=y;
                rotation[2]/=z;
            }
        }

        static glm::mat4 getTranformMatrix(const glm::mat4& transformMatrix){
            return glm::translate(glm::mat4(1.0f),  glm::vec3(transformMatrix[3]));
        }
    };

}