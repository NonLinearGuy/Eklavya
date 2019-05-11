#pragma once

#include "BaseComponent.h"
#include "../UserInputListener.h"
#include <glm/glm.hpp>

class IEventData;
class RigidBodyComponent;
class AnimationComponent;

class MovementComponent : public BaseComponent,public UserInputListener
{
public:
	MovementComponent();
	~MovementComponent();
	void Init()override;
	void Tick(float deltaTime) override;
	void Destroy()override;
	void OnKeyAction(int key,int action)override;
	/* max speed to reach*/
	inline void SetMaxSpeed(float maxSpeed) { m_MaxSpeed = maxSpeed; }
	/* will decide how fast actor will go to max speed*/
	inline void SetAcceleration(float accel) { m_Acceleration = accel; }
	/* will apply impulse in up direction*/
	inline void SetJumpForce(float jumpForce) { m_JumpForce = jumpForce; }
	/* Should movement stopped immediately when user released button*/
	inline void SetShouldStopImmediately(bool shouldStop) { m_ShouldStopImmediately = shouldStop; }
	
	ComponentID GetID()override { return s_ID; }
	static ComponentID s_ID;

	void OnCollision(std::shared_ptr<IEventData> data);

private:
	bool m_IsInAir;
	float m_MaxSpeed;
	float m_Acceleration;
	float m_JumpForce;
	bool m_ShouldStopImmediately;

	std::shared_ptr<RigidBodyComponent> m_Body;
	std::shared_ptr<AnimationComponent> m_Animator;

	glm::vec3 m_CurrentDirection;
};