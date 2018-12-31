#include "DebugCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../Timer.h"
#include <GLFW/glfw3.h>




DebugCamera::DebugCamera()
	: 
	UserInputListener(),
	m_Senstivity(.005),
	m_Pitch(-90.0f),
	m_Yaw(0.0f),
	m_Position(0.0f),
	m_Front(0.0f,0.0f,1.0f),
	m_CursorStartedMoving(true),
	m_Speed(150)
{

}


void DebugCamera::OnCursorMove(double x, double y)
{
	if (m_CursorStartedMoving)
	{
		m_LastCursorX = x;
		m_LastCursorY = y;
		m_CursorStartedMoving = false;
	}

	float xOffset = x - m_LastCursorX;
	float yOffset = y - m_LastCursorY;

	xOffset *= m_Senstivity;
	yOffset *= m_Senstivity;

	m_LastCursorX = x;
	m_LastCursorY = y;

	m_Yaw += xOffset;
	m_Pitch += yOffset;

	m_Front.x = cos(m_Yaw) * cos(m_Pitch);
	m_Front.y = sin(m_Pitch);
	m_Front.z = sin(m_Yaw) * cos(m_Pitch);

	m_Front = glm::normalize(m_Front);
}

void DebugCamera::PollKeyAction()
{
	float dt = HipHop::Timer::GetInstance()->GetDeltaTimeInSeconds();
	
	if (IsKeyPressed(GLFW_KEY_W))
		Move(EDirection::UP,dt);
	if (IsKeyPressed(GLFW_KEY_S))
		Move(EDirection::DOWN, dt);
	if (IsKeyPressed(GLFW_KEY_A))
		Move(EDirection::LEFT, dt);
	if (IsKeyPressed(GLFW_KEY_D))
		Move(EDirection::RIGHT, dt);
}

void DebugCamera::Move(EDirection direction,float dt)
{
	float speed = m_Speed * dt;

	switch (direction)
	{
	case EDirection::LEFT:
		m_Position += speed * glm::cross(glm::vec3(0.0f,1.0f,0.0f),m_Front);
		break;
	case EDirection::RIGHT:
		m_Position += speed * glm::cross(m_Front, glm::vec3(0.0f, 1.0f, 0.0f));
		break;
	case EDirection::UP:
		m_Position += speed * m_Front;
		break;
	case EDirection::DOWN:
		m_Position += speed * -m_Front;
		break;
	}
}



glm::mat4 DebugCamera::GetView()
{
	return glm::lookAt(m_Position,m_Position + m_Front,glm::vec3(0.0f,1.0f,0.0f));
}
