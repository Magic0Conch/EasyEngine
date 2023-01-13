#include "PhongLightingRenderPass.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include <memory>
#include <string>


using namespace EasyEngine;

void PhongLightingRenderPass::initialize(){
    JsonManipulation jsonManipulation(PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),"scene.json"));
    sceneConfig = jsonManipulation.getJsonData();
}

PhongLightingRenderPass::PhongLightingRenderPass(const string& shaderPath)
: Light(shaderPath,glm::vec3(1.0f,1.0f,1.0f),glm::vec3(1.2f,1.0f,2.0f)),
objectColor(1.0f,0.5f,0.31f),ambientStrength(0.1f),specularStrength(1.0f)
{
    ModelMesh lightCubeModel(PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),"light_cube_with_normal.txt"));

    glGenVertexArrays(1,&lightVAO);
    glBindVertexArray(lightVAO);
    
    glGenBuffers(1, &lightVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, lightCubeModel.meshDataSize,lightCubeModel.meshData, GL_STATIC_DRAW);
    
    glGenBuffers(1, &lightVEO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightVEO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, lightCubeModel.indicesSize, lightCubeModel.indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    diffuseMap =make_shared<Texture>(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(),"default\\steel_border_box.bmp"));
    specularMap = make_shared<Texture>(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(),"default\\steel_border_box_specular.bmp"));
    shader->use();
    diffuseMap->setTextureUnit(0);
    specularMap->setTextureUnit(1);
    shader->setValue("material.diffuseMap", 0);
    shader->setValue("material.specularMap", 1);
    shader->setValue("material.ambient",1.0f,1.0f,1.0f);
    shader->setValue("material.diffuse",1.0f,1.0f,1.0f);
    shader->setValue("material.specular",0.5f, 0.5f, 0.5f);
    shader->setValue("material.shininess",32.0f);
    shader->setValue("directionalLight.ambient", 0.2f, 0.2f, 0.2f);
    shader->setValue("directionalLight.diffuse",0.5f, 0.5f, 0.5f);
    shader->setValue("directionalLight.specular",10.0f, 10.0f, 10.0f);
    shader->setValue("directionalLight.direction",-0.2f, -1.0f, -0.3f);
    glm::vec3 pointLightPositions[] = {
	glm::vec3( 0.7f,  0.2f,  2.0f),
	glm::vec3( 2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3( 0.0f,  0.0f, -3.0f)
    };  
    for(int i = 0;i<4;++i){
        shader->setValue("pointLights["+to_string(i)+"].position",pointLightPositions[i]);
        shader->setValue("pointLights["+to_string(i)+"].ambient", 0.05f, 0.05f, 0.05f);
        shader->setValue("pointLights["+to_string(i)+"].diffuse",0.8f, 0.8f, 0.8f);
        shader->setValue("pointLights["+to_string(i)+"].specular",1.0f, 1.0f, 1.0f);
        shader->setValue("pointLights["+to_string(i)+"].constant",1.0f);
        shader->setValue("pointLights["+to_string(i)+"].linear",0.09f);
        shader->setValue("pointLights["+to_string(i)+"].quadratic",0.032f);
        shader->setValue("pointLights["+to_string(i)+"].specular",1.0f, 1.0f, 1.0f);
    }
    shader->setValue("pointLights[0].diffuse",0.8f, 0.2f, 0.2f);
    shader->setValue("pointLights[1].diffuse",0.2f, 0.8f, 0.2f);
    shader->setValue("pointLights[2].diffuse",0.2f, 0.2f, 0.8f);
    shader->setValue("pointLights[3].diffuse",0.2f, 0.8f, 0.8f);


}
void PhongLightingRenderPass::draw(Camera& camera){

    shader->use();
    
    shader->setValue("spotLights[0].position",camera.cameraPosition);
    shader->setValue("spotLights[0].direction",camera.cameraFront);
    shader->setValue("spotLights[0].ambient", 0.05f, 0.05f, 0.05f);
    shader->setValue("spotLights[0].diffuse",0.8f, 0.8f, 0.8f);
    shader->setValue("spotLights[0].specular",1.0f, 1.0f, 1.0f);
    shader->setValue("spotLights[0].constant",1.0f);
    shader->setValue("spotLights[0].linear",0.09f);
    shader->setValue("spotLights[0].quadratic",0.032f);
    shader->setValue("spotLights[0].cutOff",glm::cos(glm::radians(15.0f)));
    shader->setValue("spotLights[0].outerCutOff",glm::cos(glm::radians(20.0f)));


    shader->setValue("view",camera.getCameraPoseMatrix());
    shader->setValue("projection",camera.getProjectionMatrix());
    shader->setValue("viewPos",camera.cameraPosition);


    
    int len = sceneConfig["cubePositions"].size();
    for (int i = 0 ; i<len; ++i ) {
        model = glm::mat4(1.0f);
        Json::Value cubePositionRaw = sceneConfig["cubePositions"][i];
        glm::vec3 worldPosition = glm::vec3(cubePositionRaw[0].asFloat(), cubePositionRaw[1].asFloat(), cubePositionRaw[2].asFloat());
        model = glm::translate(model, worldPosition);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        shader->setValue("model",model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}
