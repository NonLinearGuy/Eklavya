#include "RigidBody.h"
#include <glm/gtc/matrix_transform.hpp>

RigidBody::RigidBody()
	:
	m_Position(0.0f,0.0f,0.0f),
	m_LinearVelocity(0.0f,0.0f,0.0f),
	m_LinearAcceleration(0.0f,0.0f,0.0f),
	m_ToWorld(1.0f)
{
}

RigidBody::~RigidBody()
{
}

void RigidBody::Integrate(float deltaTime)
{
	m_Position += m_LinearVelocity * deltaTime;
	m_LinearVelocity += m_LinearAcceleration * deltaTime;

	UpdateInternals();
}

void RigidBody::UpdateInternals()
{
	m_ToWorld = glm::translate(m_ToWorld,m_Position);
}
