#include "ProjectileComponent.h"
#include "Constants.h"
#include "../Components/GameActor.h"
#include "../Components/Transform.h"
#include "../Helpers.h"

using namespace std;

unsigned int ProjectileComponent::s_ID = 1000;

ProjectileComponent::ProjectileComponent()
	:
	m_Velocity(0),m_Acceleration(0),m_Position(0)
{

}

void ProjectileComponent::Init()
{
	BaseComponent::Init();

	assert(m_Owner);

	m_OwnerTransform = MakeSharedPtr(m_Owner->GetComponent<Transform>(Transform::s_ID));
}


void ProjectileComponent::Destroy()
{
	m_OwnerTransform.reset();
}

void ProjectileComponent::Tick(float dt)
{
	m_Position = m_Position + m_Velocity * dt;
	m_Acceleration = m_Acceleration;
	m_Velocity = m_Velocity + (m_Acceleration * dt);
	m_OwnerTransform->SetPosition(m_Position);
}

