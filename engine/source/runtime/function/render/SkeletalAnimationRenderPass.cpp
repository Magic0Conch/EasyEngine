#include "SkeletalAnimationRenderPass.h"
#include "runtime/resource/res_type/components/Animator.h"
#include "runtime/resource/res_type/components/Model.h"


using namespace EasyEngine;
//To get visuals similar to Phong shading the specular shininess exponent has to be set a bit higher. 
//A general rule of thumb is to set it between 2 and 4 times the Phong shininess exponent. 
SkeletalAnimationRenderPass::SkeletalAnimationRenderPass(const string& shaderPath)
: RenderPass(shaderPath),objectColor(1.0f,0.5f,0.31f),ambientStrength(0.1f),specularStrength(1.0f){}

void SkeletalAnimationRenderPass::initialize(){
    auto modelPath = PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),"backpack/backpack.obj");
    Model model(modelPath);
    // Animation animation();
    // Animator animator();
}


void SkeletalAnimationRenderPass::draw(Camera& camera){

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


    // shader->setValue("view",camera.getCameraPoseMatrix());
    // shader->setValue("projection",camera.getProjectionMatrix());
    shader->setValue("viewPos",camera.cameraPosition);


    // model = glm::mat4(1.0f);
    // shader->setValue("model",model);
    // models[0].draw(*shader);
}
