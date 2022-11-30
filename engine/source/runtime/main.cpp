#include "include/Utility.h"
#include "include/Shader.h"
#include "include/ImageProcessing.h"
#include "include/Texture.h"
#include "include/WindowTime.h"
#include "include/ModelMesh.h"
#include "include/CameraController.h"
#include "include/JsonManipulation.h"

//must include GLAD before GLFW. The include file for GLAD includes the required OpenGL headers(GL/gl.h ... )
using namespace std;

const string meshFilePath = R"(D:\work\project\2209\LearnOpenGL\LearnOpenGL_0_Start\Data\plane.txt)";
const string indiciesFilePath = R"(D:\work\project\2209\LearnOpenGL\LearnOpenGL_0_Start\Data\plane_indices.txt)";
const string sceneConfigFilePath = R"(D:\work\project\2209\LearnOpenGL\LearnOpenGL_0_Start\Data\scene.json)";
const string boxTextureFilePath = R"(D:\resources\ComputerGraphics\wooden_container.jpg)";
const string smileFaceTextureFilePath = R"(D:\resources\ComputerGraphics\awesomeface.png)";
const string triangleShaderVert = "Shaders/triangle_shader.vert";
const string triangleShaderFrag = "Shaders/triangle_shader.frag";

unsigned int VAO,VBO,EBO;
Shader* shader;
Texture* boxTexture,* smileFaceTexture;
ModelMesh* planeModelMesh;

glm::mat4 model ;
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection;
float viewportWidth = 800.0f, viewportHeight = 600.0f;

//muti object can't access the same address.

JsonManipulation jsonManipulation(sceneConfigFilePath);
GLFWwindow* window;
Camera camera;
CameraController cameraController(camera);

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	viewportWidth = width;
	viewportHeight = height;
	glViewport(0, 0, width, height);
}


void bindArrayBuffer() {
	//the buffer type of a vertex buffer object is GL_ARRAY_BUFFER.
	glGenBuffers(1, &VBO);
	//glBufferData is a function specifically targeted to copy user-defined data into the currently bound buffer. 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, planeModelMesh->meshDataSize, planeModelMesh->meshData, GL_STATIC_DRAW);
}

void bindElementArrayBuffer() {
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, planeModelMesh->indicesSize, planeModelMesh->indices, GL_STATIC_DRAW);
}

void getEnviromentInfomation() {
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
}

void loadVertexData() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	bindArrayBuffer();
	bindElementArrayBuffer();
	//tell OpenGL how to interpret the vertex data;0->set the location of the vertex attribute to 0  layout(location=0);the size of vertex attribute(vec3) stride 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
}

void initialize() {
	//prepare the vertex data
	planeModelMesh = new ModelMesh(meshFilePath, indiciesFilePath);

	loadVertexData();

	boxTexture = new Texture(boxTextureFilePath);
	smileFaceTexture = new Texture(smileFaceTextureFilePath, GL_BGRA, GL_RGBA,1);
	
	//Watch out the following texture overwrite the earlier one.
	
	//Setup shader
	shader = new Shader(triangleShaderVert.c_str(), triangleShaderFrag.c_str());
	//Use the shader program before setting the values.
	shader->use();
	shader->setValue("texture0",(int)0);
	//shader->setValue("texture1",1);
	//shader->setValue("transform", mvpMartrix);

	//OpenGL settings
	glEnable(GL_DEPTH_TEST);
}

/// <summary>
/// Draw some objects here!
/// </summary>
void setShaderArributes() {
	//projection matrix
	projection = camera.getProjectionMatrix();
	//view matrix
	view = camera.getCameraPoseMatrix();
	
	//model matrix
	Json::Value sceneConfig = jsonManipulation.getJsonData();
	int cubePositionsLen = sceneConfig["cubePositions"].size();
	for (int i = 0; i < cubePositionsLen; ++i) {
		model = glm::mat4(1.0f);
		Json::Value cubePositionRaw = sceneConfig["cubePositions"][i];
		glm::vec3 worldPosition = glm::vec3(cubePositionRaw[0].asFloat(), cubePositionRaw[1].asFloat(), cubePositionRaw[2].asFloat());
		model = glm::translate(model, worldPosition);
		//transmit the data from cpu to gpu
		boxTexture->setTextureUnit(0);		
		shader->setValue("view", view);
		shader->setValue("projection", projection);
		shader->setValue("model", model);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void mainLoop() {
	//Time
	WindowTime::updateTimeValue();
	//Events
	glfwPollEvents();
	input::InputHandler::getInstance().handleInput(window);
	cameraController.processInput();
	//render
	glClearColor(.0f, .0f, .0f, .0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	setShaderArributes();
	glfwSwapBuffers(window);
}

int main() {
	
	glfwInit(); //initialize GLFW
	//configure the options prefixed with GLFW_
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create the window
	window = glfwCreateWindow(viewportWidth, viewportHeight,  "LearnOpenGL_0_Start", NULL, NULL);//window witdth,height,name
	if (window == NULL) {
		cout << "create window failed" << endl;
		glfwTerminate();
		return -1;
	}
	//tell GLFW to make the context of our window the main context on the current threaed
	glfwMakeContextCurrent(window);

	//load the address of the OpenGL pointers
	//glfwGetProcAddress defines the correct function based on which OS we're compiling for
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "GLAD initializaion failed!" << endl;
	}

	//define the size of the rendering window
	glViewport(0, 0, viewportWidth, viewportHeight);//position, size(in pixels)
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	initialize();
	//Main render loop
	while (!glfwWindowShouldClose(window)) {		
		mainLoop();
	}
	//printf("%.6le %d\n", end - start,frameCnt);
	glfwTerminate();
	return 0;
}