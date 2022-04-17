#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(SPEED),
	m_MouseSensitivity(SENSITIVITY), m_Zoom(ZOOM)
{
	m_Position = position;
	m_WorldUp = up;
	m_Yaw = yaw;
	m_Pitch = pitch;
	UpdateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(SPEED), 
	m_MouseSensitivity(SENSITIVITY), m_Zoom(ZOOM)
{
	m_Position = glm::vec3(posX, posY, posZ);
	m_WorldUp = glm::vec3(upX, upY, upZ);
	m_Yaw = yaw;
	m_Pitch = pitch;
	UpdateCameraVectors();
}

void Camera::ProcessKeyboard(Camera_Movement direction, float delatTime)
{
	float velocity = m_MovementSpeed * delatTime;
	switch (direction)
	{
		case FORWARD:
			m_Position += m_Front * velocity;
			break;
		case BACKWARD:
			m_Position -= m_Front * velocity;
			break;
		case LEFT:
			m_Position -= m_Right * velocity;
			break;
		case RIGHT:
			m_Position += m_Right * velocity;
			break;
		case UP:
			m_Position += m_Up * velocity;
			break;
		case DOWN:
			m_Position -= m_Up * velocity;
			break;
	}
}

void Camera::ProcessMouse(float xOffset, float yOffset, GLboolean constrainPitch)
{
	xOffset *= m_MouseSensitivity;
	yOffset *= m_MouseSensitivity;

	m_Yaw += xOffset;
	m_Pitch += yOffset;

	if (constrainPitch)
	{
		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		else if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;
	}

	UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffset)
{
	m_Zoom -= (float)yOffset;
	if (m_Zoom < 1.0f)
		m_Zoom = 1.0f;
	if (m_Zoom > 45.0f)
		m_Zoom = 45.0f;
}

void Camera::UpdateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(front);
	// also re-calculate the Right and Up vector
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}
