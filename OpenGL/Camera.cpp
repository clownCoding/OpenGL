#include "Camera.h"
#include <iostream>

Camera::Camera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp):zoom(ZOOM), mouseSensitivity(SENSITIVITY), movementSpeed(SPEED),
yaw(YAW), pitch(PITCH)
{
	this->cameraPos = cameraPos;
	this->cameraFront = cameraFront;
	this->cameraUp = cameraUp;
	UpdateCameraVectors();
}

glm::mat4 Camera::GetView()
{
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::ProcessKeyboard(GLFWwindow* window, float deltaTime)
{
	float cameraSpeed = movementSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		this->cameraPos += cameraSpeed * this->cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		this->cameraPos -= cameraSpeed * this->cameraFront;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		this->cameraPos += cameraSpeed * this->cameraUp;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		this->cameraPos -= cameraSpeed * this->cameraUp;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		this->cameraPos -= glm::cross(this->cameraFront, this->cameraUp) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		this->cameraPos += glm::cross(this->cameraFront, this->cameraUp) * cameraSpeed;
}

void Camera::PrcoessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (constrainPitch) {
		if (pitch > 89.0f) {
			pitch = 89.0f;
		}
		if (pitch < -89.0f) {
			pitch = -89.0f;	
		}
	}

	UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffset)
{
	zoom -= (float)yOffset;
	if (zoom < 1.0f)
		zoom = 1.0f;
	if (zoom > 45.0f)
		zoom = 45.0f;
}

void Camera::UpdateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}
