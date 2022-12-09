#pragma once
#include "Camera.h"
#include "InputHandler.h"
#include "WindowTime.h"
namespace EasyEngine {
	class CameraController
	{
	public:
		Camera* camera;
		const glm::dvec3 moveSpeed = { 2.5,1.5,-3 };
		const glm::dvec3 rotationSpeed = { 3.0f,2.0f,25.0f };
	public:
		CameraController();
		virtual ~CameraController();
		CameraController(Camera& initialCamera);
		//horizontal mouse-movement affects the yaw and vertical mouse-movement affects the pitch
		void processInput();
	};
}