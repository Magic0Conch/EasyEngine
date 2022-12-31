#include "PhongLightingRenderPass.h"


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

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
   
     

}
void PhongLightingRenderPass::draw(Camera& camera){
       shader->use();
        shader->setValue("objectColor",objectColor);
        shader->setValue("lightColor",lightColor);
        shader->setValue("lightPosition",lightPos);
        shader->setValue("ambientStrength",ambientStrength);
        shader->setValue("specularStrength",specularStrength);
    int len = sceneConfig["cubePositions"].size();
    for (int i = 0 ; i<len; ++i ) {
        model = glm::mat4(1.0f);
        Json::Value cubePositionRaw = sceneConfig["cubePositions"][i];
        glm::vec3 worldPosition = glm::vec3(cubePositionRaw[0].asFloat(), cubePositionRaw[1].asFloat(), cubePositionRaw[2].asFloat());
        model = glm::translate(model, worldPosition);
        shader->setValue("model",model);
        shader->setValue("view",camera.getCameraPoseMatrix());
        shader->setValue("projection",camera.getProjectionMatrix());
        shader->setValue("viewPos",camera.cameraPosition);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}
