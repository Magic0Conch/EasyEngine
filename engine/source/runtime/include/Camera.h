#pragma once
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
namespace EasyEngine {
	class Camera
	{
	public:
		glm::vec3 cameraPosition;
		//yaw pitch roll
		glm::vec3 cameraEulerRotation;
		float fov,nearPlane=0.1f,farPlane=100.0f;
		//cameraDirection is actually the reverse direction in the reverse direction of what it is targeting.
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;
		glm::vec3 cameraRight;
	private:
		const glm::vec3 world_up = glm::vec3(.0f,1.0f,.0f);
		glm::vec3 targetPosition;
		glm::mat4 cameraPoseMatrix;
		glm::mat4 projectionMatrix;
	public:
		Camera();
		Camera(const glm::vec3& initialPose, const glm::vec3& initialDirection = glm::vec3(0));
		virtual ~Camera();
		Camera& operator=(const Camera&);

		void setCameraDirectionByTarget(const glm::vec3 targetPosition);
		void setCameraPosition(const glm::vec3 cameraPosition);
		void setCameraRotationByEuler(const glm::vec3 eulerRotation);
		const glm::mat4 getCameraPoseMatrix();
		const glm::mat4 getProjectionMatrix();
	};

}