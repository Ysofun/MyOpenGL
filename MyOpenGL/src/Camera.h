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
	/// 通过键盘输入来控制摄像机移动
	/// </summary>
	/// <param name="direction">规定了4个方向，-1表示前进，0表示后退，1表示向左，2表示向右
	/// 可以用FORWARD, BACKWARD, LEFT, RIGHT来表示</param>
	/// <param name="delatTime">持续时间</param>
	void ProcessKeyboard(Camera_Movement direction, float delatTime);
	void ProcessMouse(float xOffset, float yOffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(float yOffset);

	inline glm::mat4 GetViewMatrix() const { return glm::lookAt(m_Position, m_Position + m_Front, m_Up); }

private:
	void UpdateCameraVectors();
};
