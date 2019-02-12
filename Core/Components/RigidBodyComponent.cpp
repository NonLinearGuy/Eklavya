#include "RigidBodyComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../Physics/Collider.h"
#include "GameActor.h"
#include "Transform.h"
#include "../Helpers.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

unsigned int RigidBodyComponent::s_ID = 7;

RigidBodyComponent::RigidBodyComponent(std::shared_ptr<ICollider> pCollider)
	:
	m_Rotation(glm::vec3(0.0f)),
	m_Position(0.0f,0.0f,0.0f),
	m_LinearVelocity(0.0f,0.0f,0.0f),
	m_LinearAcceleration(0.0f,0.0f,0.0f),
	m_Collider(pCollider),
	m_Sleep(false),
	m_TransformMatrix(1.0f)
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


void RigidBodyComponent::UpdateData()
{
	glm::mat4 rotation = glm::toMat4(m_Rotation);
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_Position);
	m_TransformMatrix = translation * rotation;
}



glm::vec3 RigidBodyComponent::GetPointInLocalSpace(const glm::vec3& point)
{
	auto transform = MakeSharedPtr(m_Owner->GetComponent<Transform>(Transform::s_ID));
	transform->SetPosition(m_Position);
	return transform->TransformToLocal(point);
}

glm::vec3 RigidBodyComponent::GetAxis(int index)
{
	return m_TransformMatrix[index];
}

void RigidBodyComponent::Tick(float deltaTime)
{
	if (m_Sleep) {
		UpdateData();
		return;
	}

	m_Position += m_LinearVelocity * deltaTime;
	m_LinearVelocity += m_LinearAcceleration * deltaTime;

	UpdateData();
	auto transform = MakeSharedPtr(m_Owner->GetComponent<Transform>(Transform::s_ID));
	transform->SetPosition(m_Position);
}

