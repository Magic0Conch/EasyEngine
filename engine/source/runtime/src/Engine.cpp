// #include "Engine.h"
// namespace EasyEngine {
//     //must include GLAD before GLFW. The include file for GLAD includes the required OpenGL headers(GL/gl.h ... )
//     using PU = EasyEngine::PathUtility;
//     using namespace std;

//     void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
//         viewportWidth = width;
//         viewportHeight = height;
//         glViewport(0, 0, width, height);
//     }


//     void bindArrayBuffer() {
//         //the buffer type of a vertex buffer object is GL_ARRAY_BUFFER.
//         glGenBuffers(1, &VBO);
//         //glBufferData is a function specifically targeted to copy user-defined data into the currently bound buffer. 
//         glBindBuffer(GL_ARRAY_BUFFER, VBO);
//         glBufferData(GL_ARRAY_BUFFER, planeModelMesh->meshDataSize, planeModelMesh->meshData, GL_STATIC_DRAW);
//     }

//     void bindElementArrayBuffer() {
//         glGenBuffers(1, &EBO);
//         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//         glBufferData(GL_ELEMENT_ARRAY_BUFFER, planeModelMesh->indicesSize, planeModelMesh->indices, GL_STATIC_DRAW);
//     }

//     void getEnviromentInfomation() {
//         int nrAttributes;
//         glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
//         std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
//     }

//     void loadVertexData() {
//         glGenVertexArrays(1, &VAO);
//         glBindVertexArray(VAO);
//         bindArrayBuffer();
//         bindElementArrayBuffer();
//         //tell OpenGL how to interpret the vertex data;0->set the location of the vertex attribute to 0  layout(location=0);the size of vertex attribute(vec3) stride 
//         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//         glEnableVertexAttribArray(0);
//         glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
//         glEnableVertexAttribArray(1);
//     }

//     void initialize() {
//         //prepare the vertex data
//         planeModelMesh = new ModelMesh(PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),"plane.txt")
//         , PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),"plane_indices.txt"));

//         loadVertexData();

//         boxTexture = new Texture(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(),"default/wooden_container.jpg"));
//         smileFaceTexture = new Texture(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(),"default/awesomeface.jpg"), GL_BGRA, GL_RGBA,1);
        
//         //Watch out the following texture overwrite the earlier one.
        
//         //Setup shader
//         shader = new Shader(PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(),"triangle_shader.vert").c_str(),
//                                             PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(),"triangle_shader.frag").c_str());
//         //Use the shader program before setting the values.
//         shader->use();
//         shader->setValue("texture0",(int)0);
//         //shader->setValue("texture1",1);
//         //shader->setValue("transform", mvpMartrix);

//         //OpenGL settings
//         glEnable(GL_DEPTH_TEST);
//     }

//     /// <summary>
//     /// Draw some objects here!
//     /// </summary>
//     void setShaderArributes() {
//         //projection matrix
//         projection = camera.getProjectionMatrix();
//         //view matrix
//         view = camera.getCameraPoseMatrix();
        
//         //model matrix
//         Json::Value sceneConfig = jsonManipulation.getJsonData();
//         int cubePositionsLen = sceneConfig["cubePositions"].size();
//         for (int i = 0; i < cubePositionsLen; ++i) {
//             model = glm::mat4(1.0f);
//             Json::Value cubePositionRaw = sceneConfig["cubePositions"][i];
//             glm::vec3 worldPosition = glm::vec3(cubePositionRaw[0].asFloat(), cubePositionRaw[1].asFloat(), cubePositionRaw[2].asFloat());
//             model = glm::translate(model, worldPosition);
//             //transmit the data from cpu to gpu
//             boxTexture->setTextureUnit(0);		
//             shader->setValue("view", view);
//             shader->setValue("projection", projection);
//             shader->setValue("model", model);
//             //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//             glDrawArrays(GL_TRIANGLES, 0, 36);
//         }
//     }

//     void mainLoop() {
//         //Time
//         WindowTime::updateTimeValue();
//         //Events
//         glfwPollEvents();
//         input::InputHandler::getInstance().handleInput(window);
//         cameraController.processInput();
//         //render
//         glClearColor(.0f, .0f, .0f, .0f);
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//         setShaderArributes();
//         glfwSwapBuffers(window);
//     }
// }
