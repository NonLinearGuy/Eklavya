#include <glad/glad.h>
#include "MovementComponent.h"
#include "RigidBodyComponent.h"
#include "../Event/EventDispatcher.h"
#include <GLFW/glfw3.h>
#include "GameActor.h"
#include "../Helpers.h"
#include "../Debugging/Diagnostics.h"
#include "AnimationComponent.h"


ComponentID MovementComponent::s_ID = 111;

MovementComponent::MovementComponent()
{
	m_IsInAir = true;
	m_Acceleration = 200.0f;
	m_MaxSpeed = 20000.0f;
	m_ShouldStopImmediately = true;
	m_JumpForce = 1000.0f;
	m_CurrentDirection = glm::vec3(0.0f);
}

MovementComponent::~MovementComponent()
{

}

void MovementComponent::Init()
{
	if (m_Owner)
	{
		m_Body = MakeSharedPtr(m_Owner->GetComponent<RigidBodyComponent>(RigidBodyComponent::s_ID));
		
	}

	RegisterToEvent<MovementComponent>(this, &MovementComponent::OnCollision, EEventType::ON_COLLISION_ENTER);
}

void MovementComponent::Destroy()
{
	UnRegisterToEvent<MovementComponent>(this,&MovementComponent::OnCollision,EEventType::ON_COLLISION_ENTER);
}

#pragma optimize("",off)
void MovementComponent::OnCollision(std::shared_ptr<IEventData> data)
{
	auto collisionData = std::static_pointer_cast<EventOnCollisionEnter>(data);
	int id = collisionData->ActorID;
	if (collisionData->ActorID == m_Owner->GetID())
	{
		m_IsInAir = false;
	}
}
#pragma optimize("",on)


void MovementComponent::OnKeyAction(int key,int action)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
			case GLFW_KEY_SPACE:
				if (!m_IsInAir)
				{
					m_IsInAir = true;
					m_Body->ApplyLinearImpulse(glm::vec3(0.0f, m_JumpForce, 0.0f));
				}
				break;
			
			case GLFW_KEY_UP:
				m_Body->SetOrientation(glm::vec3(0.0f,0.0f,0.0f));
				m_Body->SetAccel(0.0f,m_Body->GetAccel().y,m_Acceleration);
				break;

			case GLFW_KEY_DOWN:
				m_Body->SetOrientation(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
				m_Body->SetAccel(0.0f, m_Body->GetAccel().y, -m_Acceleration);
				break;

			case GLFW_KEY_LEFT:
				m_Body->SetOrientation(glm::vec3(0.0f, glm::radians(90.0f), 0.0f));
				m_Body->SetAccel(m_Acceleration, m_Body->GetAccel().y, 0.0f);
				break;

			case GLFW_KEY_RIGHT:
				m_Body->SetOrientation(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));
				m_Body->SetAccel(-m_Acceleration, m_Body->GetAccel().y, 0.0f);
				break;
			case GLFW_KEY_G:
				m_Body->SetAccel(0.0f, m_Body->GetAccel().y,400.0f );
				break;
			case GLFW_KEY_X:
			{
				auto animator = m_Owner->GetComponent<AnimationComponent>(AnimationComponent::s_ID);
				MakeSharedPtr(animator)->PlayNextAnimation();
			}
				break;
		};
	}

	if (action == GLFW_RELEASE)
	{
		switch (key)
		{
		case GLFW_KEY_SPACE:
			break;

		case GLFW_KEY_UP:
		case GLFW_KEY_DOWN:
		case GLFW_KEY_LEFT:
		case GLFW_KEY_RIGHT:
			m_Body->SetAccel(0.0f,m_Body->GetAccel().y,0.0f);
			if (m_ShouldStopImmediately)
				m_Body->SetVel(0.0f,m_Body->GetVelocity().y,0.0f);
			
			break;
		};
	}

}

void MovementComponent::Tick(float dt)
{
	//static float angle = 0.0f;
	//angle += dt;
	//m_Body->SetOrientation(glm::vec3(0.0f,angle,0.0f));
	//DiagManager::sGeneralDiagsMap[EMapKeys::KEY_DELTA] = std::to_string(angle);
}

