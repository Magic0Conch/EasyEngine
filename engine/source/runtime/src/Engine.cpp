#include "Engine.h"

using namespace EasyEngine;



void Engine::initialize(){
    colorsRenderPass = make_shared<ColorsRenderPass>("lighting\\color");
    colorsRenderPass->initialize();
    phongLightingRenderPass = make_shared<PhongLightingRenderPass>("lighting\\phong");
    phongLightingRenderPass->initialize();
}

void Engine::tickRender() {
    projection = camera.getProjectionMatrix();
    view = camera.getCameraPoseMatrix();
    colorsRenderPass->draw(camera);
    phongLightingRenderPass->draw(camera);
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
    tickRender();
    glfwSwapBuffers(EngineWindow::getInstance().window);
}

void Engine::start(){
    EngineWindow::getInstance().createWindow();
    //load the address of the OpenGL pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "GLAD initializaion failed!" << endl;
    }    
    initialize();
    glEnable(GL_DEPTH_TEST);
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
    // jsonManipulation = JsonManipulation(PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),"scene.json"));
    cameraController = CameraController(camera);
    
}