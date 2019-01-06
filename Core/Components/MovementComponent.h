#pragma once

#include "BaseComponent.h"
#include<glm/glm.hpp>
#include "GameActor.h"
#include "Transform.h"
#include "../Helpers.h"
const float toPIE = 3.1415926535 / 180.0f;

class MovementComponent : public BaseComponent
{
public:

	ComponentID GetID() { return 14; }

	MovementComponent() : BaseComponent()
	{

	}

	~MovementComponent() 
	{
	}

	void Init()override {}
	void Destroy()override {}

	void Tick(float deltaTime)override
	{
		static float angle = 0.0f;
		angle += deltaTime;
		auto transform = MakeSharedPtr(m_Owner->GetComponent<Transform>(Transform::s_ID));
		if (transform)
		{
			glm::vec3 OldPosition = transform->GetPosition();
			OldPosition.y +=  40.0f* deltaTime * sin(angle);
			transform->SetPosition(OldPosition);
			transform->SetRotation(glm::vec3(angle,0.0f,0.0f));
		}
	}

};


class RotateXZComponent : public BaseComponent
{
public:

	ComponentID GetID() { return 15; }

	RotateXZComponent() : BaseComponent()
	{

	}

	~RotateXZComponent()
	{
	}

	void Init()override {}
	void Destroy()override {}
	void SetStartAngle(float startAngle) { m_StartAngle = startAngle; }

	void Tick(float deltaTime)override
	{
		static float angle = 0.0f;
		angle += 1.0f * deltaTime;
		auto transform = MakeSharedPtr(m_Owner->GetComponent<Transform>(Transform::s_ID));
		glm::vec3 oldPosition = transform->GetPosition();
		float x = 40 * cos(toPIE * (m_StartAngle + angle));
		float z = 40 * sin(toPIE * (m_StartAngle + angle));
		oldPosition.x = x;
		oldPosition.z = z;
		transform->SetPosition(oldPosition);
	}

private:
	float m_StartAngle;
};


class RotateYZComponent : public BaseComponent
{
public:

	ComponentID GetID() { return 18; }

	RotateYZComponent() : BaseComponent()
	{

	}

	~RotateYZComponent()
	{
	}

	void Init()override {}
	void Destroy()override {}
	void SetStartAngle(float startAngle) { m_StartAngle = startAngle; }

	void Tick(float deltaTime)override
	{
		static float angle = 0.0f;
		angle += 1.0f * deltaTime;
		auto transform = MakeSharedPtr(m_Owner->GetComponent<Transform>(Transform::s_ID));
		glm::vec3 oldPosition = transform->GetPosition();
		float y = 20 + 60 * sin(toPIE * (m_StartAngle + angle));
		float z = 60 * cos(toPIE * (m_StartAngle + angle));
		oldPosition.y = y;
		oldPosition.z = z;
		transform->SetPosition(oldPosition);
	}

private:
	float m_StartAngle;
};