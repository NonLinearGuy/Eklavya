#include "DebugCamera.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../Timer.h"
#include <GLFW/glfw3.h>
#include "../Helpers.h"
#include "../Renderer/GLRenderer.h"
#include "Frustum.h"
#include "Scene.h"


DebugCamera::DebugCamera(float fov,float ratio,float nearDist, float farDist)
	: 
	BaseNode(ACTOR_NOT_NEEDED,nullptr,ERenderGroup::OUTLINED),
	UserInputListener(),
	m_Frustum(new Frustum(fov,ratio,nearDist,farDist)),
	m_Senstivity(.005),
	m_Pitch(90.0f),
	m_Yaw(0.0f),
	m_Position(0.0f,150.0f,200.0f),
	m_Front(0.0f,-0.5f,-1.0f),
	m_CursorStartedMoving(true),
	m_Speed(450),
	m_LastCursorX(0),m_LastCursorY(0),
	m_View(1.0f),
	m_Projection(glm::perspective(fov,ratio,nearDist,farDist)),
	m_Debug(false)
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

	UpdateCamera();
}

DebugCamera::~DebugCamera()
{

}

void DebugCamera::UpdateCamera()
{
	m_View = glm::lookAt(m_Position, m_Position + m_Front, glm::vec3(0.0f, 1.0f, 0.0f));
	if (!m_Debug)
	{
		m_Frustum->UpdatePlanes(m_Projection, m_View);
	}
}

void DebugCamera::PollKeyAction()
{
	float dt = Timer::GetInstance()->GetDeltaTimeInSeconds();
	
	if (IsKeyPressed(GLFW_KEY_W))
		Move(EDirection::UP,dt);
	if (IsKeyPressed(GLFW_KEY_S))
		Move(EDirection::DOWN, dt);
	if (IsKeyPressed(GLFW_KEY_A))
		Move(EDirection::LEFT, dt);
	if (IsKeyPressed(GLFW_KEY_D))
		Move(EDirection::RIGHT, dt);
}

void DebugCamera::OnKeyAction(int key, int action)
{
	if (GLFW_KEY_LEFT_SHIFT == key && GLFW_PRESS == action)
	{
		m_Debug = !m_Debug;
		if (m_Debug)
		{
			m_ToWorld = glm::inverse(m_View);
		}
	}
}

void DebugCamera::PreRender(Scene* scene)
{
	scene->PushMatrix(m_ToWorld);
	scene->GetRenderer()->GetActiveProgram()->SetVec4("color",glm::vec4(1.0f,1.0f,0.0f,1.0f));
}

void DebugCamera::Render(Scene * scene)
{
	if(m_Debug)
		m_Frustum->Draw();
}

void DebugCamera::PostRender(Scene* scene)
{
	scene->PopMatrix();
}

void DebugCamera::Move(EDirection direction,float dt)
{
	float speed = m_Speed * dt;

	switch (direction)
	{
	case EDirection::LEFT:
		m_Position += speed * glm::cross(glm::vec3(0.0f,1.0f,0.0f),m_Front);
		break;
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

	UpdateCamera();
}


glm::mat4 DebugCamera::GetView()
{
	return m_View;
}

glm::mat4 DebugCamera::GetProjection()
{
	return m_Projection;
}

glm::mat4 DebugCamera::GetClipMatrix()
{
	return m_Projection * m_View;
}


