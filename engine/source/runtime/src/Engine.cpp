#include "Engine.h"
#include "runtime/function/render/PostProcessingPass.h"
#include "runtime/function/render/SkyboxRenderpass.h"
#include "runtime/function/render/postprocessing/MSAA.h"
#include "runtime/function/render/preprocessing/PreProcessingPass.h"
#include "runtime/function/render/scene/AsteroidField.h"
#include "runtime/function/render/test/BlendTesting.h"
#include "runtime/function/render/test/DepthTesting.h"
#include "runtime/function/render/test/ExplodingObjects.h"
#include "runtime/function/render/test/GeometryTesting.h"
#include "runtime/function/render/test/InstancingTesting.h"
#include "runtime/function/render/test/ReflectionTesting.h"
#include "runtime/function/render/test/StencilTesting.h"
#include "runtime/function/render/test/VisualizeNormalVectors.h"
#include <memory>

using namespace EasyEngine;

void Engine::initialize(){
    renderPasses.emplace_back(make_shared<PreProcessingPass>());
    // renderPasses.emplace_back(make_shared<BlendTesting>("test/blend_testing"));
    // renderPasses.emplace_back(make_shared<SkyboxRenderpass>("skybox"));
    // renderPasses.emplace_back(make_shared<ReflectionTesting>("test/reflection_testing"));
    // renderPasses.emplace_back(make_shared<GeometryTesting>("test/geometry_testing","test/geometry_testing","test/geometry_testing"));
    // renderPasses.emplace_back(make_shared<ExplodingObjects>("test/exploding_objects"));visualize_normal_vectors
    // renderPasses.emplace_back(make_shared<VisualizeNormalVectors>("common/triangle","test/visualize_normal_vectors"));
    // renderPasses.emplace_back(make_shared<InstancingTesting>("test/instancing_testing"));
    // renderPasses.emplace_back(make_shared<AsteroidField>("scene/asteroid_field"));
    renderPasses.emplace_back(make_shared<MSAA>("postprocessing/MSAA"));
    for (const shared_ptr<RenderPass> rp : renderPasses) {
        rp->initialize();
    }    
}

void Engine::tickRender() {
    projection = camera.getProjectionMatrix();
    view = camera.getCameraPoseMatrix();
    // colorsRenderPass->draw(camera);
    for (const shared_ptr<RenderPass> rp : renderPasses) {
        rp->draw(camera);
    }    
    // phongLightingRenderPass->draw(camera);
}
void Engine::mainLoop() {
    //Time
    WindowTime::updateTimeValue();
    //Events
    glfwPollEvents();
    input::InputHandler::getInstance().handleInput(EngineWindow::getInstance().window);
    cameraController.processInput();
    //render
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
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