#include "PlayerMovementComponent.h"
#include "RigidBodyComponent.h"
#include "../Event/EventDispatcher.h"
#include <GLFW/glfw3.h>
#include "GameActor.h"
#include "../Helpers.h"

ComponentID PlayerMovementComponent::s_ID = 111;

void PlayerMovementComponent::Init()
{
	m_IsInAir = true;
	m_Acceleration = glm::vec3(20.0f);
	m_Speed = 100.0f;
	m_ShouldStopImmediately = true;
	m_JumpForce = 200.0f;

	if(m_Owner)
		m_Body = MakeSharedPtr(m_Owner->GetComponent<RigidBodyComponent>(RigidBodyComponent::s_ID));

	RegisterToEvent<PlayerMovementComponent>(this, &PlayerMovementComponent::OnCollision, EEventType::ON_COLLISION_ENTER);
}

void PlayerMovementComponent::Destroy()
{
	UnRegisterToEvent<PlayerMovementComponent>(this,&PlayerMovementComponent::OnCollision,EEventType::ON_COLLISION_ENTER);
}

#pragma optimize("",off)
void PlayerMovementComponent::OnCollision(std::shared_ptr<IEventData> data)
{
	auto collisionData = std::static_pointer_cast<EventOnCollisionEnter>(data);
	int id = collisionData->ActorID;
	if (collisionData->ActorID == m_Owner->GetID())
	{
		m_Body->SetAccel(m_Body->GetAccel() * -1.0f);
	}
}
#pragma optimize("",on)


void PlayerMovementComponent::OnKeyAction(int key,int action)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && !m_IsInAir)
	{
		m_IsInAir = true;
		m_Body->ApplyLinearImpulse(glm::vec3(0.0f, m_JumpForce, 0.0f));
	}
}

void PlayerMovementComponent::Tick(float dt)
{
	
}

