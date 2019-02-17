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
				//	sphere1->GetBody()->SetSleep(true);
					//sphere2->GetBody()->SetSleep(true);
				}
				else
				{
					m_BaseNodeMap[firstCollider]->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
					m_BaseNodeMap[secondCollider]->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
				}
			}

			if (firstCollider->GetType() == EColliderType::BOX && secondCollider->GetType() == EColliderType::SPHERE)
			{
				auto box1 = std::static_pointer_cast<BoxCollider>(firstCollider);
				auto sphere2 = std::static_pointer_cast<SphereCollider>(secondCollider);

				if (ContactGenerator::SphereAndBox(box1, sphere2, m_Contacts))
				{
					m_BaseNodeMap[firstCollider]->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
					m_BaseNodeMap[secondCollider]->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
					box1->GetBody()->SetAwake(false);
					sphere2->GetBody()->SetAwake(true);
				}
				else
				{
					m_BaseNodeMap[firstCollider]->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
					m_BaseNodeMap[secondCollider]->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
				}
			}

			if (firstCollider->GetType() == EColliderType::SPHERE && secondCollider->GetType() == EColliderType::BOX)
			{
				auto box = std::static_pointer_cast<BoxCollider>(secondCollider);
				auto sphere = std::static_pointer_cast<SphereCollider>(firstCollider);

				if (ContactGenerator::SphereAndBox(box, sphere, m_Contacts))
				{
					m_BaseNodeMap[firstCollider]->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
					m_BaseNodeMap[secondCollider]->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
					box->GetBody()->SetAwake(false);
					sphere->GetBody()->SetAwake(false);
				}
				else
				{
					m_BaseNodeMap[firstCollider]->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
					m_BaseNodeMap[secondCollider]->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
				}
			}

			if (firstCollider->GetType() == EColliderType::BOX && secondCollider->GetType() == EColliderType::BOX)
			{
				auto box1 = std::static_pointer_cast<BoxCollider>(secondCollider);
				auto box2 = std::static_pointer_cast<BoxCollider>(firstCollider);

				if (IntersectionTests::BoxAndBox(box1,box2))
				{
					m_BaseNodeMap[firstCollider]->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
					m_BaseNodeMap[secondCollider]->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
					box1->GetBody()->SetAwake(false);
					box2->GetBody()->SetAwake(false);
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
