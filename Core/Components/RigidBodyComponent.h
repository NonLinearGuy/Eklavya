#pragma once

#include<glm/glm.hpp>
#include "BaseComponent.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

class ICollider;

class RigidBodyComponent : public BaseComponent
{
public:

	RigidBodyComponent(std::shared_ptr<ICollider> pCollider);
	~RigidBodyComponent();

	void Init()override;
	void Destroy()override;
	void Tick(float deltaTime)override;

	ComponentID GetID()override { return s_ID; }
	static ComponentID s_ID;

	inline void SetMass(float mass) 
	{ 
		assert(mass > .001f); 
		m_InverseMass = 1.0f / mass; 
	}
	
	inline void SetInverseMass(float mass) { m_InverseMass = mass; }
	inline void SetPos(glm::vec3 pPosition) { m_Position = pPosition; }
	inline void SetRotation(const glm::vec3& eulerAngles) { m_Rotation = glm::quat(eulerAngles);  }
	inline void SetVel(glm::vec3 pVelocity) { m_LinearVelocity = pVelocity; }
	inline void SetAccel(glm::vec3 pAccel) { m_LinearAcceleration = pAccel; }
	inline glm::vec3 GetPosition() { return m_Position; }
	inline std::shared_ptr<ICollider> GetCollider() { return m_Collider; }

	glm::vec3 GetPointInLocalSpace(const glm::vec3& point);
	
	inline void SetSleep(bool value) { m_Sleep = value; }

	glm::vec3 GetAxis(int index);

private:

	void UpdateData();

	glm::vec3 m_LinearVelocity;
	glm::vec3 m_LinearAcceleration;
	glm::vec3 m_Position;
	glm::quat m_Rotation;
	float m_InverseMass;

	std::shared_ptr<ICollider> m_Collider;

	glm::mat4 m_TransformMatrix;

	bool m_Sleep;
	
};

