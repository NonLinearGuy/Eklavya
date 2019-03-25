#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

class RigidBodyComponent;

enum class EColliderType
{
	BOX = 0,
	SPHERE,
	CYLINDER,
	PLANE,
	MAX
};

class Physics;
class ICollider
{
	friend class Physics;

public:
	ICollider(EColliderType type);
	virtual ~ICollider();
	EColliderType GetType() { return m_Type; }
	inline void SetBody(std::shared_ptr<RigidBodyComponent> pBody) { m_Body = pBody; }
	inline std::shared_ptr<RigidBodyComponent> GetBody() { return m_Body; }
protected:
	EColliderType m_Type;
	std::shared_ptr<RigidBodyComponent> m_Body;
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