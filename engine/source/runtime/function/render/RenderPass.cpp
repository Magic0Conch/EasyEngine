#include "RenderPass.h"


using namespace EasyEngine;


RenderPass::RenderPass(const std::string& shaderPath)
{
    shader = std::make_shared<Shader>(PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), shaderPath+".vert").c_str()
               ,PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), shaderPath+".frag").c_str());
}