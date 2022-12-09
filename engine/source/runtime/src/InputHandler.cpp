#include "../include/InputHandler.h"
#include "../include/WindowTime.h"
#include <iostream>
namespace EasyEngine {
using namespace input;
void input::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	InputHandler& inputHandler = InputHandler::getInstance();
	inputHandler.currentMousePosition.x = xpos;
	inputHandler.currentMousePosition.y = -ypos;

}

void input::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	InputHandler& inputHandler = InputHandler::getInstance();
	inputHandler.currentMousePosition.z = inputHandler.currentMousePosition.z+ yoffset;
}

InputHandler::InputHandler() {
	currentMousePosition = glm::vec3(viewportHeight/2, viewportWidth/2, 0);
}
InputHandler::~InputHandler() {

}

InputHandler& InputHandler::getInstance()
{
	static InputHandler instance;
	return instance;
}


void InputHandler::handleInput(GLFWwindow* window)
{
	//first mouse
	if (firstMouse) {
		lastMousePosition = currentMousePosition;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		firstMouse = false;
	}

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_KEY_DOWN) {
		glfwSetInputMode(window, GLFW_CURSOR, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_KEY_UP) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	//keyboard input
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		buttonInputAxis[ZAXIS] = 1;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		buttonInputAxis[ZAXIS] = -1;
	}
	else {
		buttonInputAxis[ZAXIS] = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		buttonInputAxis[XAXIS] = -1;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		buttonInputAxis[XAXIS] = 1;
	}
	else {
		buttonInputAxis[XAXIS] = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		buttonInputAxis[YAXIS] = 1;
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		buttonInputAxis[YAXIS] = -1;
	}
	else {
		buttonInputAxis[YAXIS] = 0;
	}
	//mouse input
	mouseOffset = currentMousePosition - lastMousePosition;
	lastMousePosition = currentMousePosition;
	mouseInputAxis = mouseOffset;
	//std::cout << mouseInputAxis.x << ":" << mouseInputAxis.y << ":" << mouseInputAxis.z << std::endl;
}
}