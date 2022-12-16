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
#include "../function/render/lighting/Colors.h"
#include <iostream>
#include <memory>
#include <string>
using PU = EasyEngine::PathUtility;

namespace EasyEngine {
	//must include GLAD before GLFW. The include file for GLAD includes the required OpenGL headers(GL/gl.h ... )
	using namespace std;
    class Engine{
    private:
        Camera camera;
        CameraController cameraController;   
        //muti object can't access the same address.
        JsonManipulation jsonManipulation;
        unsigned int VAO,VBO,EBO;
        Shader* shader;
        Texture* boxTexture,* smileFaceTexture;
        ModelMesh* planeModelMesh;

        glm::mat4 model ;
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection;

        shared_ptr<Color> colorsObject;
    public:

        Engine(const std::string&);
        void bindArrayBuffer();
        void bindElementArrayBuffer();
        void getEnviromentInfomation();
        void loadVertexData();
        void initialize();
        void setShaderArributes();
        void mainLoop();
        void start();
    };
}