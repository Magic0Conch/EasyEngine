#include "Light.h"

using namespace EasyEngine;

Light::Light(){

}

Light::Light(const std::string& shaderPath,const glm::vec3 lightColor,const glm::vec3 lightPos)
:lightColor(lightColor)
,lightPos(lightPos)
{
    shader = std::make_shared<Shader>(PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), shaderPath+".vert").c_str()
               ,PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), shaderPath+".frag").c_str());
}