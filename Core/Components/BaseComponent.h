#pragma once

#include<memory>
#include<string>
#include"../GameActor.h"

using ComponentID = unsigned int;

class BaseComponent
{
public:


	BaseComponent(const std::string& name);
	virtual ~BaseComponent();

	virtual void Init() = 0;
	virtual void Destroy() = 0;
	virtual void Tick(float deltaTime) = 0;

	inline void SetOwner(std::shared_ptr<GameActor> pOwner) { m_Owner = pOwner; }
	inline ComponentID GetID() { return m_ID; }
protected:
	std::shared_ptr<GameActor> m_Owner;
	ComponentID m_ID;
	std::string m_Name;
private:
	static unsigned int sCompCounter;
};
