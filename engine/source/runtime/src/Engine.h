#pragma once
#include "../include/Utility.h"
#include "../include/Shader.h"
#include "../include/EngineWindow.h"
#include "../include/ImageProcessing.h"
#include "../include/Texture.h"
#include "../include/WindowTime.h"
#include "../include/ModelMesh.h"
#include "../include/CameraController.h"
#include "../include/JsonManipulation.h"
#include "../function/global/global_context.h"
#include "../function/render/lighting/ColorsRenderPass.h"
#include "../function/render/lighting/PhongLightingRenderPass.h"
#include <iostream>
#include <memory>
#include <string>
using PU = EasyEngine::PathUtility;

class RenderPass;
namespace EasyEngine {
	//must include GLAD before GLFW. The include file for GLAD includes the required OpenGL headers(GL/gl.h ... )
	using namespace std;
    class Engine{
    private:
        Camera camera;
        CameraController cameraController;   
        shared_ptr<ColorsRenderPass> colorsRenderPass;
        shared_ptr<PhongLightingRenderPass> phongLightingRenderPass;

        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection;

    public:

        Engine(const std::string&);       
        void getEnviromentInfomation();
        void initialize();
        void tickRender();
        void mainLoop();
        void start();
    };
}