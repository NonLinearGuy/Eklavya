#pragma once

#include "EventType.h"
#include <memory>
#include "../Components/GameActor.h"
#include <glm/glm.hpp>
#include <vector>
#include "../Physics/CollisionDetector.h"

class IEventData
{
public:
	EEventType GetType() { return m_Type; }
protected:
	EEventType m_Type;
};

class EventActorCreated : public IEventData
{
public:
	std::shared_ptr<GameActor> m_Actor;
};

class EventActorDestroyed : public IEventData
{
public:
	std::shared_ptr<GameActor> m_GameActor;
};

class EventCollision : public IEventData
{
public:
	unsigned mFirstActorID;
	unsigned mSecondActorID;
};

class EventContactsUpdated : public IEventData
{
public:
	std::vector<ContactData> m_ContactsWorld;
};