#pragma once

#include "EventType.h"
#include <memory>
#include "../Components/GameActor.h"

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