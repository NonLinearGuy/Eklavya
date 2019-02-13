#include "ActorFactory.h"
#include "Event/EventDispatcher.h"
#include "Components/GameActor.h"
#include "Components/Transform.h"
#include "Components/BaseRenderComponent.h"
#include "Helpers.h"
#include "Globals.h"
#include "Engine.h"
#include "Components/MovementComponent.h"
#include "Components/RigidBodyComponent.h"

ActorID ActorFactory::s_ActorIDCount = 0;

const float SPEED = .01;

void ActorFactory::CreateSky()
{
	auto sky = std::make_shared<GameActor>("Sky", s_ActorIDCount);
	s_ActorIDCount++;
	auto transform = std::make_shared<Transform>();
	transform->SetOwner(sky);
	sky->AddComponent(transform);
	auto skyRenderComponent = std::make_shared<SkyRenderComponent>();
	skyRenderComponent->SetOwner(sky);
	sky->AddComponent(skyRenderComponent);
	skyRenderComponent->CreateBaseNode();

	g_Engine->AddActor(sky);

	auto data = std::make_shared<EventActorCreated>();
	data->m_Actor = sky;

	EventDispatcher::GetInstance().TriggerEvent(EEventType::ACTOR_CREATED,data);
}

void ActorFactory::CreateSphereCollider(const glm::vec3 center, float radius, glm::vec3 direction)
{
	std::string name = "SphereCollider#" + std::to_string(s_ActorIDCount);
	ActorID id = s_ActorIDCount;
	s_ActorIDCount++;

	auto newActor = std::make_shared<GameActor>(name, id);
	auto transformComponent = std::make_shared<Transform>(center);
	transformComponent->SetOwner(newActor);
	newActor->AddComponent(transformComponent);
	std::shared_ptr<BaseRenderComponent> renderComponent;
	renderComponent = std::make_shared<SphereColliderRenderComponent>(radius, 20, 20);
	renderComponent->SetOwner(newActor);
	renderComponent->SetColor(glm::vec3(1.0f));
	newActor->AddComponent(renderComponent);
	renderComponent->CreateBaseNode();
	renderComponent->GetBaseNode()->SetAlbedoName("ornament.jpg");

	auto collider = std::make_shared<SphereCollider>();
	collider->SetRadius(radius);
	auto rbComp = std::make_shared<RigidBodyComponent>(collider);
	collider->SetBody(rbComp);
	rbComp->SetPos(center);
	rbComp->SetVel(SPEED * direction);
	rbComp->SetAccel(direction);
	rbComp->SetInverseMass(radius * radius);
	rbComp->SetOwner(newActor);
	newActor->AddComponent(rbComp);

	g_Engine->AddActor(newActor);

	auto data = std::make_shared<EventActorCreated>();
	data->m_Actor = newActor;

	EventDispatcher::GetInstance().TriggerEvent(EEventType::ACTOR_CREATED, data);
}

void ActorFactory::CreateBoxCollider(const glm::vec3 position, const glm::vec3& halfSize, const glm::vec3& rotation, glm::vec3 direction,bool movement)
{
	std::string name = "BoxCollider#" + std::to_string(s_ActorIDCount);
	ActorID id = s_ActorIDCount;
	s_ActorIDCount++;

	auto newActor = std::make_shared<GameActor>(name, id);
	auto transformComponent = std::make_shared<Transform>(position,halfSize,rotation);
	transformComponent->SetOwner(newActor);
	newActor->AddComponent(transformComponent);
	std::shared_ptr<BaseRenderComponent> renderComponent;
	renderComponent = std::make_shared<BoxColliderRenderComponent>();
	renderComponent->SetOwner(newActor);
	renderComponent->CreateBaseNode();
	renderComponent->GetBaseNode()->SetAlbedoName("Debug.png");
	newActor->AddComponent(renderComponent);

	//physics component

		auto collider = std::make_shared<BoxCollider>();
		collider->SetHalfSize(halfSize);
		auto rbComp = std::make_shared<RigidBodyComponent>(collider);
		collider->SetBody(rbComp);
		rbComp->SetPos(position);
		rbComp->SetRotation(rotation);
		rbComp->SetInverseMass(40.0f);
		rbComp->SetVel(SPEED * direction);
		rbComp->SetAccel(direction);
		rbComp->SetOwner(newActor);
		if (!movement)
			rbComp->SetSleep(true);
		newActor->AddComponent(rbComp);
	
	g_Engine->AddActor(newActor);

	auto data = std::make_shared<EventActorCreated>();
	data->m_Actor = newActor;

	EventDispatcher::GetInstance().TriggerEvent(EEventType::ACTOR_CREATED, data);
}
