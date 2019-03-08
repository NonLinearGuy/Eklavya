#include "ActorFactory.h"
#include "Event/EventDispatcher.h"
#include "Components/GameActor.h"
#include "Components/PawnActor.h"
#include "Components/Transform.h"
#include "Components/BaseRenderComponent.h"
#include "Helpers.h"
#include "Globals.h"
#include "Engine.h"
#include "Components/RigidBodyComponent.h"
#include "Components/PlayerMovementComponent.h"
#include "Random.h"
#include "Animation/Animation.h"
#include "Components/AnimationComponent.h"

ActorID ActorFactory::s_ActorIDCount = 0;

const float SPEED = .1f;

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

std::shared_ptr<RigidBodyComponent> ActorFactory::CreateSphereCollider(const glm::vec3 center, 
	float radius, glm::vec3 direction,float mass)
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
	renderComponent->GetBaseNode()->SetAlbedoName("pixar.jpg");

	auto collider = std::make_shared<SphereCollider>();
	collider->SetRadius(radius);
	auto rbComp = std::make_shared<RigidBodyComponent>(collider);
	collider->SetBody(rbComp);
	rbComp->SetPos(center);
	rbComp->SetInverseMass(mass);
	rbComp->SetAccel(direction * 100.0f);
	//rbComp->SetAngularAcc(glm::vec3(.1f,0.0f,0.0f));
	rbComp->SetOwner(newActor);
	rbComp->SetAwake(true);
	newActor->AddComponent(rbComp);

	g_Engine->AddActor(newActor);

	auto data = std::make_shared<EventActorCreated>();
	data->m_Actor = newActor;

	EventDispatcher::GetInstance().TriggerEvent(EEventType::ACTOR_CREATED, data);

	return rbComp;
}

std::shared_ptr<RigidBodyComponent> ActorFactory::CreateBoxCollider(const glm::vec3 position, 
	const glm::vec3& halfSize, 
	const glm::vec3& rotation, 
	glm::vec3 direction,float mass,bool infiniteMass,bool texture2)
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
	if(texture2)
		renderComponent->GetBaseNode()->SetAlbedoName("0.png");
	else
		renderComponent->GetBaseNode()->SetAlbedoName("box.png");
	newActor->AddComponent(renderComponent);

	//physics component

		auto collider = std::make_shared<BoxCollider>();
		collider->SetHalfSize(halfSize);
		auto rbComp = std::make_shared<RigidBodyComponent>(collider);
		collider->SetBody(rbComp);
		rbComp->SetPos(position);
		rbComp->SetOrientation(rotation);
		if (!infiniteMass)
		{
			rbComp->SetMass(mass);
		}
		else
		{
			rbComp->SetInverseMass(0.0f);
		}
		rbComp->SetAccel(direction * 100.0f);
		//rbComp->SetAngularAcc(glm::vec3(.1f,0.0f,0.0f));
		rbComp->SetOwner(newActor);
		rbComp->SetAwake(infiniteMass);
		newActor->AddComponent(rbComp);
	
	g_Engine->AddActor(newActor);

	auto data = std::make_shared<EventActorCreated>();
	data->m_Actor = newActor;

	EventDispatcher::GetInstance().TriggerEvent(EEventType::ACTOR_CREATED, data);

	return rbComp;
}

void ActorFactory::CreateModelActor(const glm::vec3 & position, const glm::vec3 & scale, const glm::vec3 & rotation)
{
	std::string name = "ModelLoader#" + std::to_string(s_ActorIDCount);
	ActorID id = s_ActorIDCount;
	s_ActorIDCount++;

	auto newActor = std::make_shared<GameActor>(name, id);

	//TRANSFORM
	auto transformComponent = std::make_shared<Transform>(position, scale,rotation);
	transformComponent->SetOwner(newActor);
	newActor->AddComponent(transformComponent);

	//RENDER
	std::shared_ptr<BaseRenderComponent> renderComponent;
	renderComponent = std::make_shared<MeshRenderComponent>("mannequin/mannequin.dae");
	renderComponent->SetOwner(newActor);
	renderComponent->CreateBaseNode();
	newActor->AddComponent(renderComponent);

	
	//RIGIDBODY
	auto collider = std::make_shared<BoxCollider>();
	collider->SetHalfSize(glm::vec3(10.0f));
	auto rbComp = std::make_shared<RigidBodyComponent>(collider);
	collider->SetBody(rbComp);
	rbComp->SetPos(position);
	rbComp->SetOrientation(rotation);
	rbComp->SetMass(20.0f);
	rbComp->SetAccel(glm::vec3(0.0f,-1.0f,0.0f) * 100.0f);
	//rbComp->SetAngularAcc(glm::vec3(.1f,0.0f,0.0f));
	rbComp->SetOwner(newActor);
	rbComp->SetAwake(true);
	newActor->AddComponent(rbComp);

	//PLAYER MOVEMENT
	auto movementComp = std::make_shared<PlayerMovementComponent>();
	movementComp->SetSpeed(20.0f);
	movementComp->SetOwner(newActor);
	movementComp->Init();
	newActor->AddComponent(movementComp);

	g_Engine->AddActor(newActor);
	auto data = std::make_shared<EventActorCreated>();
	data->m_Actor = newActor;

	EventDispatcher::GetInstance().TriggerEvent(EEventType::ACTOR_CREATED, data);

	//Animation
	auto animationComponent = std::make_shared<AnimationComponent>("walking.dae");
	animationComponent->Init();
	animationComponent->SetOwner(newActor);
	newActor->AddComponent(animationComponent);


}
