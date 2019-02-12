#include "Physics.h"
#include "../Event/Events.h"
#include "../Event/EventType.h"
#include "../Event/EventDispatcher.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/GameActor.h"
#include "../Helpers.h"
#include "Collider.h"
#include "../Debugging/Diagnostics.h"
#include "CollisionDetector.h"

Physics::Physics()
{
	RegisterToEvent<Physics>(this,&Physics::OnActorCreated,EEventType::ACTOR_CREATED);
	RegisterToEvent<Physics>(this, &Physics::OnActorDestroyed, EEventType::ACTOR_DESTROYED);
}

Physics::~Physics()
{
	UnRegisterToEvent<Physics>(this, &Physics::OnActorCreated, EEventType::ACTOR_CREATED);
	UnRegisterToEvent<Physics>(this, &Physics::OnActorDestroyed, EEventType::ACTOR_DESTROYED);
}

void Physics::OnActorCreated(std::shared_ptr<IEventData> data)
{
	auto actor = std::static_pointer_cast<EventActorCreated>(data)->m_Actor;
	auto body = MakeSharedPtr(actor->GetComponent<RigidBodyComponent>(RigidBodyComponent::s_ID));
	if (body)
	{
		auto collider = body->GetCollider();
		if(collider)
			m_Colliders.push_back(collider);
	}
}

void Physics::OnActorDestroyed(std::shared_ptr<IEventData> data)
{
	auto actor = std::static_pointer_cast<EventActorCreated>(data)->m_Actor;
	auto body = MakeSharedPtr(actor->GetComponent<RigidBodyComponent>(RigidBodyComponent::s_ID));
	if (body)
	{
		auto collider = body->GetCollider();
		if (collider)
		{
			auto iter = std::find(m_Colliders.begin(), m_Colliders.end(), collider);
			if (iter != m_Colliders.end())
				m_Colliders.erase(iter);
		}
	}
	//I DOUBLE DARE YOU TO USE iter AGAIN!!!!!!!!!!!!!!!!!!!
}

void Physics::Simulate(float delta)
{
	DiagManager::sPhysicsDiagsMap[EMapKeys::KEY_COLLIDERS] = std::to_string(m_Colliders.size());
	
	//Generate Contacts
	for (auto first : m_Colliders)
	{
		for (auto second : m_Colliders)
		{
			if (first == second) continue;

			if (first->GetType() == EColliderType::SPHERE && second->GetType() == EColliderType::BOX)
			{
				auto box = std::static_pointer_cast<BoxCollider>(second);
				auto sphere = std::static_pointer_cast<SphereCollider>(first);
				if (CollisionDetector::BoxAndSphere(box, sphere))
				{
					box->GetBody()->SetSleep(true);
					sphere->GetBody()->SetSleep(true);
				}
			}

			if (first->GetType() == EColliderType::BOX && second->GetType() == EColliderType::SPHERE)
			{
				auto box = std::static_pointer_cast<BoxCollider>(first);
				auto sphere = std::static_pointer_cast<SphereCollider>(second);
				if (CollisionDetector::BoxAndSphere(box, sphere))
				{
					box->GetBody()->SetSleep(true);
					sphere->GetBody()->SetSleep(true);
				}
			}

			if (first->GetType() == EColliderType::SPHERE && second->GetType() == EColliderType::SPHERE)
			{
				auto sphere1 = std::static_pointer_cast<SphereCollider>(first);
				auto sphere2 = std::static_pointer_cast<SphereCollider>(second);
				if (CollisionDetector::SphereAndSphere(sphere1, sphere2))
				{
					sphere1->GetBody()->SetSleep(true);
					sphere2->GetBody()->SetSleep(true);
				}
			}

			if (first->GetType() == EColliderType::BOX && second->GetType() == EColliderType::BOX)
			{
				auto box1 = std::static_pointer_cast<BoxCollider>(first);
				auto box2 = std::static_pointer_cast<BoxCollider>(second);
				if (CollisionDetector::BoxAndBox(box1, box2))
				{
					box1->GetBody()->SetSleep(true);
					box2->GetBody()->SetSleep(true);
				}
			}
		}
	}


	//Resolve Constraints
}
