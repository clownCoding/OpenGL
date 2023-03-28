#pragma once
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
private:
	glm::mat4 view;
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	float yaw;
	float pitch;

	float movementSpeed;
	float mouseSensitivity;
	float zoom;
public:
	Camera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp);
	glm::mat4 GetView();
	void ProcessKeyboard(GLFWwindow* window, float deltaTime);
	void PrcoessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(float yOffset);
	inline float GetZoom() { return zoom; };
	inline glm::vec3 GetPos() { return cameraPos; };
	inline glm::vec3 GetFront() { return cameraFront; };
private:
	void UpdateCameraVectors();
};
