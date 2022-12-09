// #pragma once
// #include "../include/Utility.h"
// #include "../include/Shader.h"
// #include "../include/ImageProcessing.h"
// #include "../include/Texture.h"
// #include "../include/WindowTime.h"
// #include "../include/ModelMesh.h"
// #include "../include/CameraController.h"
// #include "../include/JsonManipulation.h"
// #include "../function/global/global_context.h"
// #include <iostream>
// #include <string>

// namespace EasyEngine {
//     class Engine{

//         unsigned int VAO,VBO,EBO;
//         Shader* shader;
//         Texture* boxTexture,* smileFaceTexture;
//         ModelMesh* planeModelMesh;

//         glm::mat4 model ;
//         glm::mat4 view = glm::mat4(1.0f);
//         glm::mat4 projection;
//         float viewportWidth = 800.0f, viewportHeight = 600.0f;

//         //muti object can't access the same address.

//         JsonManipulation jsonManipulation;
//         GLFWwindow* window;
//         Camera camera;
//         CameraController cameraController(camera);    
//         void mainLoop();
//         void setShaderArributes();
//         void initialize();
//         void loadVertexData();
//         void getEnviromentInfomation();
//         void bindElementArrayBuffer();
//         void bindArrayBuffer();
//         void framebufferSizeCallback(GLFWwindow* window, int width, int height);
//     }
// }