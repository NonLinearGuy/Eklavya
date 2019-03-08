#pragma once

#include<glm/glm.hpp>
#include <memory>
#include "Globals.h"

enum EActorType
{
	SKY,
	SPHERE_COLLIDER,
	BOX_COLLIDER
};

class RigidBodyComponent;

class ActorFactory
{
private:
	static unsigned int s_ActorIDCount;

public:
	static void CreateSky();
	static std::shared_ptr<RigidBodyComponent> CreateSphereCollider(const glm::vec3 center,
		float radius,
		glm::vec3 direction,float mass);
	static std::shared_ptr<RigidBodyComponent> CreateBoxCollider(const glm::vec3 position,
		const glm::vec3& size, 
		const glm::vec3& rotation, 
		glm::vec3 direction,float mass,
		bool infiniteMass = false,bool texture2 = false);
	static void CreateModelActor(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation);
};