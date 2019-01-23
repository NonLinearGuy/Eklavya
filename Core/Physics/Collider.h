#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <../Components/Transform.h>
#include "../Components/BaseComponent.h"

class RigidBody;
class Texture2D;

enum class EColliderType
{
	BOX = 0,
	SPHERE,
	CYLINDER,
	PLANE,
	MAX
};

class ICollider : public BaseComponent
{
public:
	ICollider(EColliderType type);
	virtual ~ICollider();
	EColliderType GetType() { return m_Type; }

	void Init()override;
	void Tick(float delta)override;
	void Destroy()override;

	ComponentID GetID()override { return sID; }

	glm::vec3 TransformToLocal(const glm::vec3& worldPos);
	glm::vec3 GetPosition();
	std::shared_ptr<RigidBody> GetBody() { return m_RigidBody; }
protected:
	static ComponentID sID;
	std::shared_ptr<RigidBody> m_RigidBody;
	EColliderType m_Type;
};


class SphereCollider : public ICollider
{
public:
	SphereCollider();
	~SphereCollider();
	float GetRadius() { return m_Radius; }
	void SetRadius(float radius);
private:
	float m_Radius;
};

class BoxCollider : public ICollider
{
public:
	BoxCollider();
	~BoxCollider();
	glm::vec3 GetHalfSize() { return m_HalfSize; }
	void SetHalfSize(glm::vec3 halfSize);
private:
	glm::vec3 m_HalfSize;
};

struct PlaneCollider
{
	glm::vec3 m_Normal;
	float d;
};