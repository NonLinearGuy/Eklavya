#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

class Texture2D;

enum class EColliderType
{
	BOX = 0,
	SPHERE,
	CYLINDER,
	PLANE,
	MAX
};

class ICollider
{
public:
	ICollider(EColliderType type);
	virtual ~ICollider();
	EColliderType GetType() { return m_Type; }
	protected:
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