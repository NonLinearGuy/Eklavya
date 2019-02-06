#pragma once

#include<glm/glm.hpp>
#include "BaseComponent.h"

class ICollider;

class RigidBodyComponent : public BaseComponent
{
public:

	RigidBodyComponent(std::shared_ptr<ICollider> pCollider);
	~RigidBodyComponent();

	void Init()override;
	void Destroy()override;
	void Tick(float deltaTime)override;

	inline void SetMass(float mass) 
	{ 
		assert(mass > .001f); 
		m_InverseMass = 1.0f / mass; 
	}
	
	inline void SetInverseMass(float mass) { m_InverseMass = mass; }
	inline void SetPos(glm::vec3 pPosition) { m_Position = pPosition; }
	inline void SetVel(glm::vec3 pVelocity) { m_LinearVelocity = pVelocity; }
	inline void SetAccel(glm::vec3 pAccel) { m_LinearAcceleration = pAccel; }
	inline glm::vec3 GetPosition() { return m_Position; }

	ComponentID GetID()override { return s_ID; }
	static ComponentID s_ID;

private:

	glm::vec3 m_LinearVelocity;
	glm::vec3 m_LinearAcceleration;
	glm::vec3 m_Position;
	float m_InverseMass;

	std::shared_ptr<ICollider> m_Collider;
	
};

