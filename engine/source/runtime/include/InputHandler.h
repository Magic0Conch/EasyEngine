#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "WindowTime.h"
#include <iostream>
#include "EngineWindow.h"
namespace EasyEngine {

	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	class InputHandler
	{
	friend void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	friend void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	private:
		glm::dvec3 currentMousePosition;
		glm::dvec3 lastMousePosition;
		glm::dvec3 mouseOffset;
		bool firstMouse = true;

		bool isPressedG =false;
		bool isPressedESC =false;
		
	public:
		enum Axis {
			XAXIS, YAXIS, ZAXIS
		};
		glm::dvec3 buttonInputAxis;
		glm::dvec3 mouseInputAxis;
		bool gammaEnabled = false;
		bool closeWindow = false;

	private:
		InputHandler();
		virtual ~InputHandler();

	public:
		static InputHandler& getInstance();
		void handleInput(GLFWwindow* window);
	};

}