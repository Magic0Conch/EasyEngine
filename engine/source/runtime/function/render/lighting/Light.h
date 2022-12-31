#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/fwd.hpp"
#include "../../../include/Shader.h"
#include "../../global/global_context.h"

#include <glm/glm.hpp>
#include <memory>
#include <string.h>
namespace EasyEngine {
    using PU = EasyEngine::PathUtility;

    class Light{
    protected:
        unsigned int lightVAO;
        unsigned int lightVBO;
        unsigned int lightVEO;

        std::shared_ptr<Shader> shader;

        glm::vec3 lightColor;
        glm::vec3 lightPos;


    public:
        Light();
        Light(const std::string& shaderPath,const glm::vec3 lightColor,const glm::vec3 lightPos);
    };

}