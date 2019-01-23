#include "EventDispatcher.h"
#include <cassert>

EventDispatcher::~EventDispatcher()
{
}

EventDispatcher::EventDispatcher()
{
	m_Listeners.reserve(EEventType::MAX);
	for (int type = 0; type < EEventType::MAX; ++type)
	{
		m_Listeners[type].reserve(10);
	}
}


void EventDispatcher::AddListener(Callback listener,EEventType eventType)
{
	assert(eventType >= EEventType::MIN && eventType < EEventType::MAX);
	auto iter = std::find(m_Listeners[eventType].begin(), m_Listeners[eventType].end() , listener);
	if (iter != m_Listeners[eventType].end()) return;
	m_Listeners[eventType].push_back(listener);
}

void EventDispatcher::RemoveListener(Callback listener, EEventType eventType)
{
	assert(eventType >= EEventType::MIN && eventType < EEventType::MAX);
	auto iter = std::find(m_Listeners[eventType].begin(), m_Listeners[eventType].end(), listener);
	if (iter == m_Listeners[eventType].end()) return;
	m_Listeners[eventType].erase(iter);
}

void EventDispatcher::TriggerEvent(EEventType eventType,EventDataSharedPtr eventData)
{
	assert(eventType >= EEventType::MIN && eventType < EEventType::MAX);

	for (auto callback : m_Listeners[eventType])
		callback(eventData);
}