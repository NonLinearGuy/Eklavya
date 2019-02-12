#include "Collider.h"
#include "../Helpers.h"
#include "../Components/GameActor.h"


ICollider::ICollider(EColliderType type)
	:
	m_Type(type)
{
}

ICollider::~ICollider()
{
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
	int a = 2 + 3;
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
}
