#include "PreProcessingPass.h"
#include "glm/fwd.hpp"
#include "runtime/function/render/RenderPass.h"
#include "runtime/include/EngineWindow.h"
#include <cstddef>
namespace EasyEngine {
    void PreProcessingPass::initialize(){
        glGenBuffers(1,&uboMatrices);
        glBindBuffer(GL_UNIFORM_BUFFER,uboMatrices);
        glBufferData(GL_UNIFORM_BUFFER,2*sizeof(glm::mat4),NULL,GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER,0);
        glBindBufferRange(GL_UNIFORM_BUFFER,0,uboMatrices,0,2*sizeof(glm::mat4));

    }
    void PreProcessingPass::draw(Camera& camera){
        glBindBuffer(GL_UNIFORM_BUFFER,uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(glm::mat4),glm::value_ptr(camera.getProjectionMatrix()));
        glBufferSubData(GL_UNIFORM_BUFFER,sizeof(glm::mat4),sizeof(glm::mat4),glm::value_ptr(camera.getCameraPoseMatrix()));
        glBindBuffer(GL_UNIFORM_BUFFER,0);
    }
    PreProcessingPass::PreProcessingPass(){}
}