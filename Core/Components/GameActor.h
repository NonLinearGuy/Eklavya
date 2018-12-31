#ifndef _INC_GAME_ACTOR_H_
#define _INC_GAME_ACTOR_H_

#include <map>
#include <memory>
#include <string>


using ActorID = unsigned int;
using ComponentID = unsigned int;

class BaseComponent;

class GameActor
{
public:
	using ComponentMap = std::map<ComponentID , std::shared_ptr<BaseComponent> >;

	explicit GameActor(const std::string& name,ActorID id);
	~GameActor();

	bool Init();
	void Destroy();
	void Tick(float deltaTime);
	void AddComponent(std::shared_ptr<BaseComponent> NewComponent);
	void RemoveComponent(ComponentID id);
	
	template<typename ComponentType>
	std::weak_ptr<ComponentType> GetComponent(ComponentID id)
	{
		auto iter = m_Components.find(id);
		if (iter != m_Components.end())
		{
			std::shared_ptr<BaseComponent> baseType = iter->second;
			std::shared_ptr<ComponentType> subType = std::static_pointer_cast<ComponentType>(baseType);
			std::weak_ptr<ComponentType> weakPtr(subType);
			return weakPtr;
		}
		else
			return std::weak_ptr<ComponentType>();
	}

	inline ActorID GetID() { return m_ID; }

protected:
	std::string m_Name;
	ActorID m_ID;
	ComponentMap m_Components;	
};

#endif