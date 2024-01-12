#pragma once
#include <iostream>
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtx/quaternion.hpp"
namespace EasyEngine {
    class Math{
    public:
        Math();
        
        template<class T>
        static T lerp(T a,T b,T f){
            return a + f*(b-a);
        }

        static glm::vec3 projectOntoVector(const glm::vec3& v, const glm::vec3& onto) {
            return glm::dot(v, onto) / glm::dot(onto, onto) * onto;
        }
        
        static void decomposeVector3D(const glm::vec3 v0,const glm::vec3 v1,const glm::vec3 targerVector,glm::vec3& outV0,glm::vec3& outV1){
            glm::vec3 parallelComponent1 = projectOntoVector(targerVector, v0);
            glm::vec3 parallelComponent2 = projectOntoVector(targerVector, v1);
            outV0 = parallelComponent1 + parallelComponent2;
            glm::vec3 perpendicularComponent = targerVector - parallelComponent1 - parallelComponent2;
            outV1 = perpendicularComponent;
            std::cout << "Parallel Component 1: (" << outV0.x << ", " << outV0.y << ", " << outV0.z << ")" << std::endl;
            std::cout << "Perpendicular Component: (" << outV1.x << ", " << outV1.y << ", " << outV1.z << ")" << std::endl;

        }

        static glm::mat4 getTranformMatrix(const glm::mat4& transformMatrix){
            return glm::translate(glm::mat4(1.0f),  glm::vec3(transformMatrix[3]));
        }

        template<class T>
        static T bilinearInterpolation(float u, float v,const T& m_bottomLeft,const T& m_bottomRight,const T& m_topLeft,const T& m_topRight) {
            const T result = glm::mix(glm::mix(m_bottomLeft, m_bottomRight, u),glm::mix(m_topLeft,m_topRight, u), v);
            return result;
        }

        static glm::mat4 getRotationMatrix(const glm::mat4& inputMatrix){
            glm::vec3 scale;
            scale.x = glm::length(glm::vec3(inputMatrix[0])); 
            scale.y = glm::length(glm::vec3(inputMatrix[1])); 
            scale.z = glm::length(glm::vec3(inputMatrix[2])); 

            glm::mat4 rotationMatrix = inputMatrix;
            rotationMatrix[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); 
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    rotationMatrix[i][j] /= scale[i]; 
                }
            }
            return rotationMatrix;

        };
    };

}