#pragma once

#include "BaseComponent.h"
#include<glm/glm.hpp>
#include "GameActor.h"
#include "Transform.h"
#include "../Helpers.h"

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
			OldPosition.y +=  deltaTime * sin(angle);
			transform->SetPosition(OldPosition);
			transform->SetRotation(glm::vec3(angle,0.0f,0.0f));
		}
	}

};