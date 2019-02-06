#include "RigidBodyComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../Physics/Collider.h"
#include "GameActor.h"
#include "Transform.h"
#include "../Helpers.h"

unsigned int RigidBodyComponent::s_ID = 7;

RigidBodyComponent::RigidBodyComponent(std::shared_ptr<ICollider> pCollider)
	:
	m_Position(0.0f,0.0f,0.0f),
	m_LinearVelocity(0.0f,0.0f,0.0f),
	m_LinearAcceleration(0.0f,0.0f,0.0f),
	m_Collider(pCollider)
{
}

RigidBodyComponent::~RigidBodyComponent()
{
}

void RigidBodyComponent::Init()
{
}

void RigidBodyComponent::Destroy()
{
}

void RigidBodyComponent::Tick(float deltaTime)
{
	m_Position += m_LinearVelocity * deltaTime;
	m_LinearVelocity += m_LinearAcceleration * deltaTime;

	auto transform = MakeSharedPtr(m_Owner->GetComponent<Transform>(Transform::s_ID));
	transform->SetPosition(m_Position);
}

