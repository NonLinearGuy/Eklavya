#pragma once

#include<glm/glm.hpp>

class RigidBody
{
public:
	RigidBody();
	~RigidBody();
	void Integrate(float deltaTime);
	inline void SetMass(float mass) 
	{ 
		assert(mass > .001f); 
		m_InverseMass = 1.0f / mass; 
	}
	
	inline void SetInverseMass(float mass) { m_InverseMass = mass; }
	inline void SetInitialPos(glm::vec3 pPosition) { m_Position = pPosition; }
	inline void SetInitialVel(glm::vec3 pVelocity) { m_LinearVelocity = pVelocity; }
	inline void SetInitialAccel(glm::vec3 pAccel) { m_LinearAcceleration = pAccel; }

	inline glm::vec3 GetPosition() { return m_Position; }

	glm::mat4 GetModel() { return m_ToWorld; }
	
private:
	void UpdateInternals();

	glm::vec3 m_LinearVelocity;
	glm::vec3 m_LinearAcceleration;
	glm::vec3 m_Position;
	float m_InverseMass;
	glm::mat4 m_ToWorld;
};

