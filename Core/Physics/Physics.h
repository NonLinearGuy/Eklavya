#pragma once

#include <memory>
#include <vector>

//Forward Declarations
class ICollider;
class IEventData;

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
};