#pragma once
#include <glad/glad.h>
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <string>
#include "../../../include/ModelMesh.h"
#include "../../../include/Shader.h"
#include "../../global/global_context.h"
#include "../RenderPass.h"
#include "Light.h"
namespace EasyEngine {
    using PU = EasyEngine::PathUtility;

    class PhongLightingRenderPass:RenderPass,Light{
    private:
        glm::vec3 resultColor;
        glm::vec3 objectColor;

        float ambientStrength;
        float specularStrength;

    public:
        PhongLightingRenderPass(const string& shaderPath);
        virtual void draw(Camera& camera);
        // virtual void draw(const glm::mat4& viewMatrix,const glm::mat4& projectionMatrix,const glm::vec3& viewPos);
        virtual void initialize();
    };
}