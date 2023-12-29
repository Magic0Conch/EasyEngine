#include "Engine.h"
#include "runtime/function/render/DeferredLightingRenderpass.h"
#include "runtime/function/render/GeometryRenderpass.h"
#include "runtime/function/render/PostProcessingPass.h"
#include "runtime/function/render/SkeletalAnimationRenderPass.h"
#include "runtime/function/render/SkyboxRenderpass.h"
#include "runtime/function/render/lighting/PhongLightingRenderPass.h"
#include "runtime/function/render/lighting/ShadowMapping.h"
#include "runtime/function/render/lighting/ShadowMappingPoint.h"
#include "runtime/function/render/postprocessing/Bloom.h"
#include "runtime/function/render/postprocessing/MSAA.h"
#include "runtime/function/render/preprocessing/PreProcessingPass.h"
#include "runtime/function/render/scene/AsteroidField.h"
#include "runtime/function/render/test/BlendTesting.h"
#include "runtime/function/render/test/DepthTesting.h"
#include "runtime/function/render/test/ExplodingObjects.h"
#include "runtime/function/render/test/GammaCorrectionTesting.h"
#include "runtime/function/render/test/GeometryTesting.h"
#include "runtime/function/render/test/InstancingTesting.h"
#include "runtime/function/render/test/PBRTesting.h"
#include "runtime/function/render/test/ParallexTesting.h"
#include "runtime/function/render/test/ReflectionTesting.h"
#include "runtime/function/render/test/SSAOTest.h"
#include "runtime/function/render/test/StencilTesting.h"
#include "runtime/function/render/test/VisualizeNormalVectors.h"
#include "runtime/function/render/test/NormalmapTesting.h"
#include "runtime/include/InputHandler.h"
#include <iostream>
#include <memory>
#include <string>

using namespace EasyEngine;

void Engine::initialize(){
    
    renderPasses.emplace_back(make_shared<PreProcessingPass>());
    // renderPasses.emplace_back(make_shared<PhongLightingRenderPass>("lighting/phong"));
    renderPasses.emplace_back(make_shared<SkeletalAnimationRenderPass>("common/animation"));
    // renderPasses.emplace_back(make_shared<BlendTesting>("test/blend_testing"));
    // renderPasses.emplace_back(make_shared<SkyboxRenderpass>("skybox"));
    // renderPasses.emplace_back(make_shared<ReflectionTesting>("test/reflection_testing"));
    // renderPasses.emplace_back(make_shared<GeometryTesting>("test/geometry_testing","test/geometry_testing","test/geometry_testing"));
    // renderPasses.emplace_back(make_shared<ExplodingObjects>("test/exploding_objects"));
    // renderPasses.emplace_back(make_shared<VisualizeNormalVectors>("common/triangle","test/visualize_normal_vectors"));
    // renderPasses.emplace_back(make_shared<InstancingTesting>("test/instancing_testing"));
    // renderPasses.emplace_back(make_shared<AsteroidField>("scene/asteroid_field"));
    // renderPasses.emplace_back(make_shared<MSAA>("postprocessing/MSAA"));
    // renderPasses.emplace_back(make_shared<DepthTesting>("test/depth_testing"));
    // renderPasses.emplace_back(make_shared<ShadowMapping>("lighting/shadow_render"));
    // renderPasses.emplace_back(make_shared<ShadowMappingPoint>("lighting/shadow_render_point"));
    // renderPasses.emplace_back(make_shared<NormalmapTesting>("test/normalmap_testing"));
    // renderPasses.emplace_back(make_shared<ParallexTesting>("test/parallex_testing"));
    // renderPasses.emplace_back(make_shared<Bloom>());
    // shared_ptr<GeometryRenderpass> grsp = make_shared<GeometryRenderpass>("common/geometry");
    // renderPasses.emplace_back(grsp);
    // renderPasses.emplace_back(make_shared<DeferredLightingRenderpass>("postprocessing/screen","test/deferred_lighting",grsp));
    // shared_ptr<GeometryRenderpass> grsp_view = make_shared<GeometryRenderpass>("common/geometry_view");
    // renderPasses.emplace_back(grsp_view);
    // renderPasses.emplace_back(make_shared<SSAOTest>("postprocessing/screen","postprocessing/SSAO",grsp_view));
    // renderPasses.emplace_back(make_shared<PBRTesting>("common/pbr"));



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
    InputHandler::getInstance().handleInput(EngineWindow::getInstance().window);
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
    while (!glfwWindowShouldClose(EngineWindow::getInstance().window) && !InputHandler::getInstance().closeWindow) {		
        mainLoop();
    }
    glfwTerminate();
}

Engine::Engine(){
    //load config file
    // cout<<BINARY_ROOT_DIR<<endl;
    printf("aa%s\n", BINARY_ROOT_DIR);
    const std::string configPath = PU::getFullPath(BINARY_ROOT_DIR, "configs/global_config.ini");
    g_global_context.initializeContext(configPath);	
    // jsonManipulation = JsonManipulation(PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),"scene.json"));
    cameraController = CameraController(camera);
    
}

int main(int argc,char** argv) {
	EasyEngine::Engine engine;
	engine.start();
    
	return 0;
}