#pragma once

#include "BaseComponent.h"
#include "../UserInputListener.h"
#include <glm/glm.hpp>

class IEventData;
class RigidBodyComponent;

class PlayerMovementComponent : public BaseComponent,public UserInputListener
{
public:
	
	void Init()override;
	void Tick(float deltaTime) override;
	void Destroy()override;

	void OnKeyAction(int key,int action)override;
	
	
	/* max speed to reach*/
	inline void SetSpeed(float speed) { m_Speed = speed; }
	/* will decide how fast actor will go to max speed*/
	inline void SetAcceleration(const glm::vec3& value) { m_Acceleration = value; }
	/* will apply impulse in up direction*/
	void SetJumpForce(float jumpForce); 
	/* Should movement stopped immediately when user released button*/
	void SetShouldStopImmediately(bool shouldStop); 
	

	ComponentID GetID()override { return s_ID; }
	static ComponentID s_ID;

	void OnCollision(std::shared_ptr<IEventData> data);

private:
	bool m_IsInAir;
	float m_Speed;
	glm::vec3 m_Acceleration;
	float m_JumpForce;
	bool m_ShouldStopImmediately;

	std::shared_ptr<RigidBodyComponent> m_Body;
};