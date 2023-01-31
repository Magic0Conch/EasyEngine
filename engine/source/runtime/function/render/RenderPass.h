#pragma once
#include "runtime/include/Shader.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <string>
#include "../global/global_context.h"
#include "../../resource/res_type/components/Camera.h"
#include "../../include/JsonManipulation.h"
namespace EasyEngine {
    using PU = EasyEngine::PathUtility;
    extern GlobalContext g_global_context;
    
    class RenderPass{
    public:
        Json::Value sceneConfig;
        glm::mat4 model;
        std::shared_ptr<Shader> shader;

    public:
        virtual void initialize()=0;
        virtual void draw(Camera& camera)=0;
        RenderPass(const std::string& shaderPath);
    };

}