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

	void SetMass(float mass);
	void SetInverseMass(float inverseMass);
	inline float GetInverseMass() { return m_InverseMass; }
	glm::mat3 GetTensor() 
	{ 
		return m_InverseTensorWorld;
	}
	void UpdateTensor();

	inline void SetPos(glm::vec3 pPosition) { m_Position = pPosition; }

	inline void SetOrientation(const glm::vec3& eulerAngles) { m_Orientation = glm::quat(eulerAngles);  }
	inline void SetVel(glm::vec3 pVelocity) { m_LinearVelocity = pVelocity; }
	inline void SetAngularVel(glm::vec3 pVel) { m_Rotation = pVel; };
	inline void SetAccel(glm::vec3 pAccel) 
	{
		m_LinearAcceleration = pAccel; 
		m_LinearVelocity *= 0.0f;
	}
	inline glm::vec3 GetAccel() { return m_LinearAcceleration; }
	inline void SetAngularAcc(glm::vec3 pAccel) { m_AngularAcceleration = pAccel; }
	inline glm::vec3 GetVelocity() { return m_LinearVelocity; }
	inline glm::vec3 GetAngularVel() { return m_Rotation; }
	inline void ApplyLinearImpulse(const glm::vec3& impulse)
	{
		m_IsAwake = true;
		m_LinearVelocity += impulse;
	}

	void AddVelocity(glm::vec3 value) 
	{ 
		m_LinearVelocity += value; 
		int a = 2 + 3;
	}
	void AddAngularVelocity(glm::vec3 value) { m_Rotation += value; }

	inline glm::vec3& GetPosition() { return m_Position; }
	inline std::shared_ptr<ICollider> GetCollider() { return m_Collider; }

	glm::vec3 GetPointInLocalSpace(const glm::vec3& point);
	glm::vec3 GetPointInWorldSpace(const glm::vec3& point);
	
	inline void SetAwake(bool isAwake) { m_IsAwake = isAwake; }

	inline void AddTorque(glm::vec3 torque) { m_Torques += torque; m_IsAwake = true; }
	inline void AddForce(glm::vec3 force) { m_Forces += force; m_IsAwake = true; }

	glm::vec3 GetAxis(int index);

private:

	void UpdateData();
	void SetTensorForSphere();
	void SetTensorForCuboid();
	void SetTensorIdentity();

	//forces
	glm::vec3 m_Torques;
	glm::vec3 m_Forces;

	//object props
	float m_InverseMass;
	float m_Mass;

	//linear motion
	glm::vec3 m_LinearVelocity;
	glm::vec3 m_LinearAcceleration;
	glm::vec3 m_Position;
	float m_LinearDamping;

	//Angular motion
	
	glm::vec3 m_Rotation;
	glm::vec3 m_AngularAcceleration;
	glm::quat m_Orientation;
	glm::mat3 m_InverseTensor;
	glm::mat3 m_InverseTensorWorld;
	float m_AngularDamping;
	
	std::shared_ptr<ICollider> m_Collider;

	glm::mat4 m_TransformMatrix;
	bool m_IsAwake;
};

