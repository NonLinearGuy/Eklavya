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
#include "../Components/BaseRenderComponent.h"
#include "../Scene/BaseNode.h"
#include "../Components/BaseComponent.h"

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
		if (collider)
		{
			m_Colliders.push_back(collider);
			auto baseNode = MakeSharedPtr(actor->GetComponent<BaseRenderComponent>(BaseRenderComponent::s_ID))->GetBaseNode();
			m_BaseNodeMap[collider] = baseNode;
		}
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
		else
		{
			int a = 2 + 3;
		}
	}
	//I DOUBLE DARE YOU TO USE iter AGAIN!!!!!!!!!!!!!!!!!!!
}

void Physics::Simulate(float delta)
{
	DiagManager::sPhysicsDiagsMap[EMapKeys::KEY_COLLIDERS] = std::to_string(m_Colliders.size());
	
	//Generate Contacts
	std::vector<ContactData> contacts;

	for (auto first = m_Colliders.begin(); first != m_Colliders.end() ;  ++first)
	{
		for (auto second = m_Colliders.begin(); second != m_Colliders.end();  ++second)
		{
			if (*first == *second) continue;

			auto firstCollider = *first;
			auto secondCollider = *second;

			if (firstCollider->GetType() == EColliderType::BOX && secondCollider->GetType() == EColliderType::BOX)
			{
				auto box1 = std::static_pointer_cast<BoxCollider>(firstCollider);
				auto box2 = std::static_pointer_cast<BoxCollider>(secondCollider);

				if (ContactGenerator::BoxAndBox(box1,box2,contacts))
				{
					box1->GetBody()->SetAwake(false);
					box2->GetBody()->SetAwake(false);
					
					std::shared_ptr<EventCollision> data = std::make_shared<EventCollision>();
					data->mFirstActorID = box1->GetBody()->GetOwnerID();
					data->mSecondActorID = box1->GetBody()->GetOwnerID();
					EventDispatcher::GetInstance().TriggerEvent(EEventType::COLLISION,data);
				}
			}

		}
	}

	
	//Resolve Constraints
}
