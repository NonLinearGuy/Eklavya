#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "../Components/BaseComponent.h"

class Transform;

class ProjectileComponent : public BaseComponent
{
public:
	ProjectileComponent();
	void Init()override;
	void Destroy()override;
	void Tick(float deltaTime)override;
	inline unsigned int GetID()override { return s_ID; }
	static unsigned int s_ID;

	inline void SetAcceleration(const glm::vec3& acc) { m_Acceleration = acc; }
	inline void SetVelocity(const glm::vec3& vel) { m_Velocity = vel; }
	inline void SetPosition(const glm::vec3& pos) { m_Position = pos; }
	inline void SetInverseMass(float pMass) { m_InverseMass = pMass; }

private:
	std::shared_ptr<Transform> m_OwnerTransform;
	glm::vec3 m_Velocity;
	glm::vec3 m_Acceleration;
	glm::vec3 m_Position;
	float m_InverseMass; 
};