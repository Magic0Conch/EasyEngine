#pragma once
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "../global/global_context.h"
#include "../../include/Camera.h"
#include "../../include/JsonManipulation.h"
namespace EasyEngine {
    using PU = EasyEngine::PathUtility;
    extern GlobalContext g_global_context;
    
    class RenderPass{
    public:
        Json::Value sceneConfig;
        glm::mat4 model;

    public:
        virtual void initialize()=0;
        virtual void draw(Camera& camera)=0;
        RenderPass();
    };

}