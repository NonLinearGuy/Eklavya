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
	}
	//I DOUBLE DARE YOU TO USE iter AGAIN!!!!!!!!!!!!!!!!!!!
}

void Physics::Simulate(float delta)
{
	DiagManager::sPhysicsDiagsMap[EMapKeys::KEY_COLLIDERS] = std::to_string(m_Colliders.size());
	
	int counter = 0;
	//Generate Contacts
	std::vector<ContactData> m_Contacts;

	for (auto first = m_Colliders.begin(); first != m_Colliders.end() ;  ++first)
	{
		for (auto second = m_Colliders.begin(); second != m_Colliders.end();  ++second)
		{
			if (*first == *second) continue;

			auto firstCollider = *first;
			auto secondCollider = *second;

			if (firstCollider->GetType() == EColliderType::SPHERE && secondCollider->GetType() == EColliderType::SPHERE)
			{
				auto sphere1 = std::static_pointer_cast<SphereCollider>(firstCollider);
				auto sphere2 = std::static_pointer_cast<SphereCollider>(secondCollider);
				
				if (ContactGenerator::SphereAndSphere(sphere1, sphere2,m_Contacts))
				{
					m_BaseNodeMap[firstCollider]->SetColor(glm::vec3(1.0f,0.0f,0.0f));
					m_BaseNodeMap[secondCollider]->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
					//sphere1->GetBody()->SetSleep(true);
					//sphere2->GetBody()->SetSleep(true);
				}
				else
				{
					m_BaseNodeMap[firstCollider]->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
					m_BaseNodeMap[secondCollider]->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
				}
			}

			counter++;
		}
	}


	//Resolve Constraints
}
