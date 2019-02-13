#pragma once

#include <memory>
#include <vector>
#include <map>

//Forward Declarations
class ICollider;
class IEventData;

//REMOVE IT LATER!!
class BaseNode;

class Physics
{
public:
	Physics();
	~Physics();
	void OnActorCreated(std::shared_ptr<IEventData> data);
	void OnActorDestroyed(std::shared_ptr<IEventData> data);
	void Simulate(float delta);
private:
	std::vector< std::shared_ptr<ICollider> > m_Colliders;
	std::map< std::shared_ptr<ICollider>,std::shared_ptr<BaseNode>> m_BaseNodeMap;

	
};