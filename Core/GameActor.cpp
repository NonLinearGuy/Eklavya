#include "GameActor.h"

GameActor::GameActor(const std::string& name,ActorID id)
	: m_Name("name"),m_ID(id)
{

}

GameActor::~GameActor()
{
}

bool GameActor::Init()
{
	for (ComponentMap::iterator iter = m_Components.begin(); iter != m_Components.end(); ++iter)
	{
		iter->second->Init();
	}
	return false;
}

void GameActor::Destroy()
{
	for (ComponentMap::iterator iter = m_Components.begin(); iter != m_Components.end(); ++iter)
	{
		iter->second->Destroy();
	}
}

void GameActor::Tick(float deltaTime)
{
	for (ComponentMap::iterator iter = m_Components.begin(); iter != m_Components.end(); ++iter)
	{
		iter->second->Tick(deltaTime);
	}
}

void GameActor::AddComponent(std::shared_ptr<BaseComponent> NewComponent)
{
	ComponentID id = NewComponent->GetID();
	m_Components[id] = NewComponent;
}

void GameActor::RemoveComponent(ComponentID id)
{
	ComponentMap::iterator position = m_Components.find(id);
	m_Components.erase(position);
}
