#include "Engine.h"
#include <memory>

using namespace EasyEngine;

/// <summary>
/// Draw some objects here!
/// </summary>
void Engine::setShaderArributes() {
    projection = camera.getProjectionMatrix();
    view = camera.getCameraPoseMatrix();
    Json::Value sceneConfig = jsonManipulation.getJsonData();
    int cubePositionsLen = sceneConfig["cubePositions"].size();
    for (int i = 0; i < cubePositionsLen; ++i) {
        model = glm::mat4(1.0f);
        Json::Value cubePositionRaw = sceneConfig["cubePositions"][i];
        glm::vec3 worldPosition = glm::vec3(cubePositionRaw[0].asFloat(), cubePositionRaw[1].asFloat(), cubePositionRaw[2].asFloat());
        model = glm::translate(model, worldPosition);
        colorsObject->setModelMatrix(model);
        colorsObject->setViewMatrix(view);
        colorsObject->setProjectionMatrix(projection);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void Engine::mainLoop() {
    //Time
    WindowTime::updateTimeValue();
    //Events
    glfwPollEvents();
    input::InputHandler::getInstance().handleInput(EngineWindow::getInstance().window);
    cameraController.processInput();
    //render
    glClearColor(.0f, .0f, .0f, .0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setShaderArributes();
    glfwSwapBuffers(EngineWindow::getInstance().window);
}

void Engine::start(){
    EngineWindow::getInstance().createWindow();
    //load the address of the OpenGL pointers
    //glfwGetProcAddress defines the correct function based on which OS we're compiling for
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "GLAD initializaion failed!" << endl;
    }    
    //prepare the file
    colorsObject = make_shared<Color>("lighting\\color");
    //Main render loop
    while (!glfwWindowShouldClose(EngineWindow::getInstance().window)) {		
        mainLoop();
    }
    glfwTerminate();
}

Engine::Engine(const std::string& binaryPath){
    //load config file
    const std::string configPath = PU::getFullPath(PU::getRootPath(binaryPath), "global_config.ini");
    g_global_context.initializeContext(configPath);	
    jsonManipulation = JsonManipulation(PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),"scene.json"));
    cameraController = CameraController(camera);
}