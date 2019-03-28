#pragma once

#include<memory>
#include<string>
#include "../Utils/Logger.h"

using ComponentID = unsigned int;

class GameActor;

class BaseComponent
{
public:
	BaseComponent();
	virtual ~BaseComponent();

	virtual ComponentID GetID() = 0;

	virtual void Init() {}
	virtual void Destroy() 
	{
		Logger::GetInstance().Log("\nDestroying " + std::string(std::to_string(GetID())));
	}
	virtual void Tick(float deltaTime) {}

	inline void SetOwner(std::shared_ptr<GameActor> pOwner) { m_Owner = pOwner; }
	unsigned GetOwnerID();

protected:
	std::shared_ptr<GameActor> m_Owner;
};


class AudioComponent : public BaseComponent
{
public:
private:
};
