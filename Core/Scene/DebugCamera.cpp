#include "DebugCamera.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../Timer.h"
#include <GLFW/glfw3.h>
#include "../Helpers.h"
#include "../Renderer/GLRenderer.h"
#include "Frustum.h"
#include "Scene.h"
#include "../Debugging/Diagnostics.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "../Utils/Logger.h"


DebugCamera::DebugCamera(float fov,float ratio,float nearDist, float farDist)
	: 
	BaseNode(ACTOR_NOT_NEEDED,nullptr,ERenderGroup::OUTLINED),
	UserInputListener(),
	m_Frustum(new Frustum(fov,ratio,nearDist,farDist)),
	m_Senstivity(.005),
	m_Pitch(90.0f),
	m_Yaw(0.0f),
	m_Position(0.0f,500.0f,500.0f),
	m_Front(0.0f,-0.5f,1.0f),
	m_CursorStartedMoving(true),
	m_Speed(650),
	m_LastCursorX(0),m_LastCursorY(0),
	m_View(1.0f),
	m_Projection(glm::perspective(fov,ratio,nearDist,farDist)),
	m_Debug(false),
	m_InterpSpeed(.03f),
	m_Interpolating(false),
	m_InterpWeight(0.0f)
{
	UpdateCamera();
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
	float dt = Timer::GetInstance().GetDeltaTimeInSeconds();
	
	if (m_Interpolating) return;
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

	if (GLFW_KEY_I == key && GLFW_PRESS == action)
	{
		m_Interpolating = true;
		m_InterpWeight = 0.0f;
		m_CursorStartedMoving = false;
	}

	if (key == GLFW_KEY_X)
	{
		
	}
}


void DebugCamera::Tick(Scene* scene, float deltaTime)
{
	if (m_Interpolating)
	{
		m_InterpWeight += m_InterpSpeed * deltaTime;
		m_Position = glm::mix(m_Position,glm::vec3(0.0f,500.0f,800.0f),m_InterpWeight);
		auto orientation = glm::quat(m_Front);
		auto result = glm::eulerAngles(glm::slerp(orientation, glm::quat(glm::vec3(0.0f, -.5f, -1.0f)), m_InterpWeight));
		m_Front.x = result.x;
		m_Front.y = result.y;
		m_Front.z = result.z;
		UpdateCamera();
	
		float insideSphere = glm::length2(glm::vec3(0.0f, 500.0f, 800.0f) - m_Position) < 10.0f;

		if (insideSphere)
		{
			m_Interpolating = false;
			m_InterpWeight = 0.0f;
			m_Yaw = result.y;
			m_Pitch = 180 - result.x;
		}
	}
}


void DebugCamera::PreRender(Scene* scene)
{
	if (!m_Target || m_Debug)
	{
		scene->PushMatrix(m_ToWorld);
		scene->GetRenderer()->GetActiveProgram()->SetVec4("color", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	}
	if(m_Target && !m_Debug)
	{
		m_Position = m_Target->GetPosition() + glm::vec3(50.0f,250.0f,-450.0f);
		UpdateCamera();
	}
}

void DebugCamera::Render(Scene * scene)
{
	if(m_Debug)
		m_Frustum->Draw();
}

void DebugCamera::PostRender(Scene* scene)
{
	if(!m_Target || m_Debug)
		scene->PopMatrix();
}

void DebugCamera::Move(EDirection direction,float dt)
{
	if (m_Target && !m_Debug) return;

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


