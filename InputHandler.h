#pragma once
#include <glfw3.h>
#include <glm/glm.hpp>

extern float viewportWidth, viewportHeight;


namespace input {
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


	public:
		enum Axis {
			XAXIS, YAXIS, ZAXIS
		};
		glm::dvec3 buttonInputAxis;
		glm::dvec3 mouseInputAxis;

	private:
		InputHandler();
		virtual ~InputHandler();

	public:
		static InputHandler& getInstance();
		void handleInput(GLFWwindow* window);
	};
}


