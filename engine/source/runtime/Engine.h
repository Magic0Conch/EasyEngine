#pragma once
#include "include/Utility.h"
#include "include/Shader.h"
#include "include/EngineWindow.h"
#include "include/ImageProcessing.h"
#include "include/WindowTime.h"
// #include "../include/ModelMesh.h"
#include "include/CameraController.h"
#include "include/JsonManipulation.h"
#include "function/global/global_context.h"
#include "function/render/lighting/ColorsRenderPass.h"
#include "function/render/lighting/PhongLightingRenderPass.h"
#include <iostream>
#include <memory>
#include <string>
#include <runtime/core/base/macro.h>

#ifdef BINARY_ROOT_DIR
// #define BINARY_ROOT_DIR_C BINARY_ROOT_DIR
#endif
using PU = EasyEngine::PathUtility;

class RenderPass;
namespace EasyEngine {
	//must include GLAD before GLFW. The include file for GLAD includes the required OpenGL headers(GL/gl.h ... )
	using namespace std;
    class Engine{
    private:
        Camera camera;
        CameraController cameraController;   
        vector<shared_ptr<RenderPass>> renderPasses;

        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection;
        std::string binaryRootDir;
    public:

        Engine();       
        void getEnviromentInfomation();
        void initialize();
        void tickRender();
        void mainLoop();
        void start();
    };
}