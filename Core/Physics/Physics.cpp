#include "Physics.h"
#include "../Event/Events.h"
#include "../Event/EventType.h"
#include "../Event/EventDispatcher.h"


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
	int a = 2 + 3;
}

void Physics::OnActorDestroyed(std::shared_ptr<IEventData> data)
{
	int a = 2 + 3;
}

void Physics::Simulate(float delta)
{
	//Generate Contacts

	//Resole
}
