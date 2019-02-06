#pragma once

#include <memory>
#include <vector>

//Forward Declarations
class Collider;
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
	std::vector< std::shared_ptr<Collider> > m_Colliders;
};