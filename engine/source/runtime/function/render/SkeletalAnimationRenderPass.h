#pragma once
#include <glad/glad.h>
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include "runtime/resource/res_type/components/Animation.h"
#include "runtime/resource/res_type/components/Animator.h"
#include "runtime/resource/res_type/components/Model.h"
#include "../global/global_context.h"
#include "RenderPass.h"
namespace EasyEngine {
    using PU = EasyEngine::PathUtility;

    class SkeletalAnimationRenderPass:public RenderPass{
    public:
        SkeletalAnimationRenderPass(const string& shaderPath);
        virtual void draw(Camera& camera);
        virtual void initialize();
    private:
        glm::vec3 resultColor;
        glm::vec3 objectColor;

        float ambientStrength;
        float specularStrength;

        std::shared_ptr<Animation> animation;
        std::shared_ptr<Animator> animator;
        std::shared_ptr<Model> model;
    };
}