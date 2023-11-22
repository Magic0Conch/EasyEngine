#include "runtime/core/base/macro.h"
#include "PBRTesting.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "runtime/function/render/RenderPass.h"
#include "runtime/function/render/basic_shape/RenderShape.h"
#include "runtime/include/EngineWindow.h"
#include "runtime/include/Shader.h"
#include "runtime/resource/res_type/components/Texture.h"
// #include <GL/gl.h>
// #include <GL/glext.h>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <memory>
#include <opencv2/imgcodecs.hpp>
#include <sys/types.h>

using namespace EasyEngine;

PBRTesting::PBRTesting(const std::string shaderPath):RenderPass(shaderPath),width(EngineWindow::getInstance().viewportWidth),height(EngineWindow::getInstance().viewportHeight){
    glCheckError();
    
    shaderEquirect = std::make_shared<Shader>(PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "common/equirect_cubemap.vert").c_str()
            ,PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "common/equirect_cubemap.frag").c_str());
    uniformBlockIndex = glGetUniformBlockIndex(shaderEquirect->ID,"Matrices");
    glUniformBlockBinding(shaderEquirect->ID,uniformBlockIndex,0);
        glCheckError();
    shaderSkybox = std::make_shared<Shader>(PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "skybox.vert").c_str()
            ,PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "skybox.frag").c_str());
    shaderIrradiance = std::make_shared<Shader>(PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "common/equirect_cubemap.vert").c_str()
            ,PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "test/cubemap_convolution.frag").c_str());
        glCheckError();
    prefilterShader = std::make_unique<Shader>(PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "common/equirect_cubemap.vert").c_str()
            ,PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "test/cubemap_convolution_specular.frag").c_str());
    brdfShader = std::make_unique<Shader>(PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "postprocessing/screen.vert").c_str()
            ,PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "test/brdf_integration.frag").c_str());
}

PBRTesting::~PBRTesting(){

}

void generateCubeMap(unsigned int& cubemap,int width,int height){
    glGenTextures(1, &cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
    for (unsigned int i = 0; i < 6; ++i){
        // note that we store each face with 16 bit floating point values
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void PBRTesting::initialize(){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);  
    glDepthFunc(GL_LEQUAL); 
    glGenFramebuffers(1, &captureFBO);
    
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    generateCubeMap(envCubemap,512,512);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    generateCubeMap(envCubemapMipmap,512,512);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    cv::Mat data = ImageProcessing::readImageByPath(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(), "skybox/newport_loft.hdr"),IMREAD_UNCHANGED,true);
    glGenTextures(1,&textureHDR);
    glBindTexture(GL_TEXTURE_2D,textureHDR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, data.cols, data.rows, 0, GL_BGR, GL_FLOAT, data.data); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data.release();
    glCheckError();

    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f),1.0f,0.1f,10.0f);
    glm::mat4 captureViews[] = {
        glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f,0.0f,0.0f), glm::vec3(0.0f,-1.0f,0.0f)),
        glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(-1.0f,0.0f,0.0f), glm::vec3(0.0f,-1.0f,0.0f)),
        glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f), glm::vec3(0.0f,0.0f,1.0f)),
        glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,-1.0f,0.0f), glm::vec3(0.0f,0.0f,-1.0f)),
        glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,1.0f), glm::vec3(0.0f,-1.0f,0.0f)),
        glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,-1.0f), glm::vec3(0.0f,-1.0f,0.0f)),
    };

    /* HDR equirectangular environment map to cubemap equivalent begin*/
    shaderEquirect->use();
    shaderEquirect->setValue("equirectangularMap",0);
    shaderEquirect->setValue("projection",captureProjection);
    glCheckError();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textureHDR);
    glViewport(0,0,512,512);
    glBindFramebuffer(GL_FRAMEBUFFER,captureFBO);
    for (unsigned int i = 0; i<6; ++i) {
        shaderEquirect->setValue("view",captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,envCubemap,0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "ERROR::FRAMEBUFFER:: captureFBO is not complete!" << std::endl;
        }
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        RenderShape::instance().renderCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    /*env cubemap end*/

    /* Mipmap HDR equirectangular environment map to cubemap equivalent begin*/
    shaderEquirect->use();
    shaderEquirect->setValue("equirectangularMap",0);
    shaderEquirect->setValue("projection",captureProjection);
    glCheckError();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textureHDR);
    glViewport(0,0,512,512);
    glBindFramebuffer(GL_FRAMEBUFFER,captureFBO);
    for (unsigned int i = 0; i<6; ++i) {
        shaderEquirect->setValue("view",captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,envCubemapMipmap,0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "ERROR::FRAMEBUFFER:: captureFBO is not complete!" << std::endl;
        }
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        RenderShape::instance().renderCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemapMipmap);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    /*env cubemap end*/
    
    /*init lights info and pbr shader parameter*/
    const unsigned int NR_LIGHTS = 1;
    for (unsigned int i = 0; i < NR_LIGHTS; i++)
    {
        lightPositions.push_back(glm::vec3(0.0, 0.0, 0.0));
        lightColors.push_back(glm::vec3(150, 150, 150));
    }
    shader->use();
    shader->setValue("albedoMap",0);
    shader->setValue("normalMap",1);
    shader->setValue("metallicMap",2);
    shader->setValue("roughnessMap",3);
    shader->setValue("aoMap",4);
    shader->setValue("irradianceMap",5);
    shader->setValue("prefilterMap",6);
    shader->setValue("brdfLUT",7);
    glCheckError();

    albedoMap = make_shared<Texture>(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(), "default/rustediron/rustediron2_basecolor.png"),GL_TEXTURE_2D,DEFAULT);
    normalMap = make_shared<Texture>(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(), "default/rustediron/rustediron2_normal.png"),GL_TEXTURE_2D,DEFAULT);
    metallicMap = make_shared<Texture>(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(), "default/rustediron/rustediron2_metallic.png"),GL_TEXTURE_2D,DEFAULT);
    roughnessMap = make_shared<Texture>(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(), "default/rustediron/rustediron2_roughness.png"),GL_TEXTURE_2D,DEFAULT);
    aoMap = make_shared<Texture>(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(), "default/rustediron/rustediron2_ao.png"),GL_TEXTURE_2D,DEFAULT);
    shaderSkybox->use();
    shaderSkybox->setValue("skybox",0);
    glCheckError();
    /*init lights info and pbr shader parameter end*/

    /*generate irradianceMap begin*/
    glGenTextures(1,&irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    for (unsigned int i = 0; i<6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, 
                    GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glCheckError();

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
    /*generate irradianceMap end*/

    /*diffuse image ibl*/
    shaderIrradiance->use();
    shaderIrradiance->setValue("environmentMap",0);
    shaderIrradiance->setValue("projection",captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP,envCubemap);
    glViewport(0, 0, 32, 32);
    glBindFramebuffer(GL_FRAMEBUFFER,captureFBO);
    for (unsigned int i = 0; i < 6; ++i){
        shaderIrradiance->setValue("view",captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "ERROR::FRAMEBUFFER:: captureFBO is not complete!" << std::endl;
        }
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        RenderShape::instance().renderCube();
        
    }
        // shaderIrradiance->setValue("view",glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,-1.0f,0.0f)));
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                        GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, irradianceMap, 0);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    /*diffuse image ibl end*/

    /*generate prefilterMap begin*/
    glGenTextures(1,&prefilterMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP,prefilterMap);
    for (unsigned int i = 0; i<6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGB16F,128,128,0,GL_RGB,GL_FLOAT,nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    /*generate prefilterMap end*/

    /*prefilter*/
    prefilterShader->use();
    prefilterShader->setValue("environmentMap",0);
    prefilterShader->setValue("projection",captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP,envCubemapMipmap);

    glBindFramebuffer(GL_FRAMEBUFFER,captureFBO);
    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip<maxMipLevels; mip++) {
        unsigned int mipWidth = 128*pow(0.5,mip);
        unsigned int mipHeight = 128*pow(0.5,mip);
        glBindRenderbuffer(GL_RENDERBUFFER,captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0,0,mipWidth,mipHeight);

        float roughness = (float)mip/(float)(maxMipLevels-1);
        prefilterShader->setValue("roughness",roughness);
        for (unsigned int i = 0; i < 6; ++i){
            prefilterShader->setValue("view",captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
            RenderShape::instance().renderCube();
            
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);

    glGenTextures(1,&brdfLUTTexture);
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

    glViewport(0, 0, 512, 512);
    brdfShader->use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    RenderShape::instance().renderQuad();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);  
    glViewport(0,0,width,height);


}

void PBRTesting::draw(Camera &camera){
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, albedoMap->id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalMap->id);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, metallicMap->id);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, roughnessMap->id);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, aoMap->id);
    glm::mat4 model = glm::mat4(1.0f);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);


    for (unsigned int i = 0; i < lightPositions.size(); i++){
        shader->setValue("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
        shader->setValue("lights[" + std::to_string(i) + "].Color", lightColors[i]);
        const float constant = 1.0f; 
        const float linear = 0.7f;
        const float quadratic = 1.8f;
        shader->setValue("lights[" + std::to_string(i) + "].Linear", linear);
        shader->setValue("lights[" + std::to_string(i) + "].Quadratic", quadratic);
    }

    for (int row = 0; row < nrRows; ++row){
        for (int col = 0; col < nrColumns; ++col){
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(
                (float)(col - (nrColumns*1.0 / 2)) * spacing,
                (float)(row - (nrRows*1.0 / 2)) * spacing,
                0.0f
            ));
            shader->setValue("model", model);
            RenderShape::instance().renderSphere();
        }
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP,prefilterMap);
    shaderSkybox->use();
    RenderShape::instance().renderCube();
    
}
