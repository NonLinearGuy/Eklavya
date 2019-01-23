#include "Collider.h"
#include "RigidBody.h"
#include "../Helpers.h"
#include "../Components/GameActor.h"

ComponentID ICollider::sID = 7;

ICollider::ICollider(EColliderType type)
	:
	BaseComponent()
{
}

ICollider::~ICollider()
{
}

void ICollider::Init()
{
	m_RigidBody = std::make_shared<RigidBody>();
}

void ICollider::Destroy()
{
	
}

glm::vec3 ICollider::TransformToLocal(const glm::vec3 & worldPos)
{
	auto transform = MakeSharedPtr(m_Owner->GetComponent<Transform>(Transform::s_ID));
	return transform->TransformToLocal(worldPos);
}

glm::vec3 ICollider::GetPosition()
{
	return m_RigidBody->GetPosition();
}


void ICollider::Tick(float delta)
{
	m_RigidBody->Integrate(delta);

	auto transform = MakeSharedPtr(m_Owner->GetComponent<Transform>(Transform::s_ID));
	if(transform)
		transform->SetPosition(m_RigidBody->GetPosition());
}

BoxCollider::BoxCollider()
	:
	ICollider(EColliderType::BOX)
{

}

BoxCollider::~BoxCollider()
{
}


void BoxCollider::SetHalfSize(glm::vec3 halfSize)
{
	m_HalfSize = glm::vec3(halfSize.x / 2.0f, halfSize.y / 2.0f, halfSize.z / 2.0f);
	auto transform = MakeSharedPtr(m_Owner->GetComponent<Transform>(Transform::s_ID));
	if (transform)
		transform->SetScale(halfSize);
}


SphereCollider::SphereCollider()
	:
	ICollider(EColliderType::SPHERE)
{
}

SphereCollider::~SphereCollider()
{
}

void SphereCollider::SetRadius(float radius)
{
	m_Radius = radius;
	auto transform = MakeSharedPtr(m_Owner->GetComponent<Transform>(Transform::s_ID));
	if (transform)
		transform->SetScale(radius);
}
