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

void ActorFactory::CreateSphereCollider(const glm::vec3 & center, float radius)
{
	std::string name = "SphereCollider#" + std::to_string(s_ActorIDCount);
	ActorID id = s_ActorIDCount;
	s_ActorIDCount++;

	auto newActor = std::make_shared<GameActor>(name, id);
	auto transformComponent = std::make_shared<Transform>();
	transformComponent->SetOwner(newActor);
	newActor->AddComponent(transformComponent);
	std::shared_ptr<BaseRenderComponent> renderComponent;
	renderComponent = std::make_shared<SphereColliderRenderComponent>(radius, 20, 20);
	renderComponent->SetOwner(newActor);
	renderComponent->SetColor(glm::vec3(1.0f));
	newActor->AddComponent(renderComponent);
	renderComponent->CreateBaseNode();

	//physics component
	auto collider = std::make_shared<SphereCollider>();
	collider->SetRadius(radius);
	auto rbComp = std::make_shared<RigidBodyComponent>(collider);
	rbComp->SetOwner(newActor);
	newActor->AddComponent(rbComp);

	g_Engine->AddActor(newActor);

	auto data = std::make_shared<EventActorCreated>();
	data->m_Actor = newActor;

	EventDispatcher::GetInstance().TriggerEvent(EEventType::ACTOR_CREATED, data);
}

void ActorFactory::CreateBoxCollider(const glm::vec3 & position, const glm::vec3& halfSize)
{
	std::string name = "BoxCollider#" + std::to_string(s_ActorIDCount);
	ActorID id = s_ActorIDCount;
	s_ActorIDCount++;

	auto newActor = std::make_shared<GameActor>(name, id);
	auto transformComponent = std::make_shared<Transform>(position,halfSize);
	transformComponent->SetOwner(newActor);
	newActor->AddComponent(transformComponent);
	std::shared_ptr<BaseRenderComponent> renderComponent;
	renderComponent = std::make_shared<BoxColliderRenderComponent>();
	renderComponent->SetOwner(newActor);
	renderComponent->CreateBaseNode();
	renderComponent->GetBaseNode()->SetAlbedoName("Debug.png");
	newActor->AddComponent(renderComponent);

	auto movementComp = std::make_shared<RotateYZComponent>();
	movementComp->SetOwner(newActor);
	newActor->AddComponent(movementComp);

	//physics component
	auto collider = std::make_shared<BoxCollider>();
	collider->SetHalfSize(halfSize/2.0f);
	auto rbComp = std::make_shared<RigidBodyComponent>(collider);
	rbComp->SetPos(position);
	rbComp->SetVel(glm::vec3(0.0f,-4.0f,0.0f));
	rbComp->SetAccel(glm::vec3(0.0f,-1.0f,0.0f));
	rbComp->SetOwner(newActor);
	newActor->AddComponent(rbComp);

	g_Engine->AddActor(newActor);

	auto data = std::make_shared<EventActorCreated>();
	data->m_Actor = newActor;

	EventDispatcher::GetInstance().TriggerEvent(EEventType::ACTOR_CREATED, data);
}
