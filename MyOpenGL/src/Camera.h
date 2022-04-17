#pragma once

#include <glad/glad.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

const float YAW =			-90.0f;
const float PITCH =			  0.0f;
const float SPEED =			  2.5f;
const float SENSITIVITY =	0.001f;
const float ZOOM =			 45.0f;

class Camera
{
public:
	glm::vec3 m_Position;
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;

	float m_Yaw;
	float m_Pitch;

	float m_MovementSpeed;
	float m_MouseSensitivity;
	float m_Zoom;

public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW, float pitch = PITCH);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	/// <summary>
	/// ͨ����������������������ƶ�
	/// </summary>
	/// <param name="direction">�涨��4������-1��ʾǰ����0��ʾ���ˣ�1��ʾ����2��ʾ����
	/// ������FORWARD, BACKWARD, LEFT, RIGHT����ʾ</param>
	/// <param name="delatTime">����ʱ��</param>
	void ProcessKeyboard(Camera_Movement direction, float delatTime);
	void ProcessMouse(float xOffset, float yOffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(float yOffset);

	inline glm::mat4 GetViewMatrix() const { return glm::lookAt(m_Position, m_Position + m_Front, m_Up); }

private:
	void UpdateCameraVectors();
};
