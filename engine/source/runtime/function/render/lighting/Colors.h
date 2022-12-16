#pragma once
#include <glad/glad.h>
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <string>
#include "../../../include/ModelMesh.h"
#include "../../../include/Shader.h"
#include "../../global/global_context.h"
namespace EasyEngine {
class Color{
using PU = EasyEngine::PathUtility;
private:
    unsigned int lightVAO;
    unsigned int lightVBO;
    unsigned int lightVEO;
    glm::vec3 resultColor;
public:
    Shader colorShader;
    glm::vec3 lightColor;
    glm::vec3 objectColor;
    glm::vec3 lightPos;
public:
    Color(const string& shaderPath):
    colorShader(PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), shaderPath+".vert").c_str()
               ,PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), shaderPath+".frag").c_str())
    ,objectColor(1.0f,0.5f,0.31f)
    ,lightColor(1.0f,1.0f,1.0f)
    ,lightPos(1.2f,1.0f,2.0f)
    {
        ModelMesh lightCubeModel(PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),"plane.txt")
                            , PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),"plane_indices.txt"));

        glGenVertexArrays(1, &lightVAO);
        glBindVertexArray(lightVAO);
        //the buffer type of a vertex buffer object is GL_ARRAY_BUFFER.
        glGenBuffers(1, &lightVBO);
        //glBufferData is a function specifically targeted to copy user-defined data into the currently bound buffer. 
        glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
        glBufferData(GL_ARRAY_BUFFER, lightCubeModel.meshDataSize,lightCubeModel.meshData, GL_STATIC_DRAW);
        
        glGenBuffers(1, &lightVEO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightVEO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, lightCubeModel.indicesSize, lightCubeModel.indices, GL_STATIC_DRAW);

        //tell OpenGL how to interpret the vertex data;0->set the location of the vertex attribute to 0  layout(location=0);the size of vertex attribute(vec3) stride 
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
   
        colorShader.use();
        colorShader.setValue("objectColor",objectColor);
        colorShader.setValue("lightColor",lightColor);
    }

    void setModelMatrix(const glm::mat4& modelMatrix){
        colorShader.setValue("model",glm::translate(modelMatrix, lightPos));
    }
    void setViewMatrix(const glm::mat4& view){
        colorShader.setValue("view", view);
    }

    void setProjectionMatrix(const glm::mat4& projection){
        colorShader.setValue("projection", projection);
    }
};
}