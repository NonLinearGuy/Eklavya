#pragma once

#include<glm/glm.hpp>
#include "Globals.h"

enum EActorType
{
	SKY,
	SPHERE_COLLIDER,
	BOX_COLLIDER
};

class ActorFactory
{
private:
	static unsigned int s_ActorIDCount;

public:
	static void CreateSky();
	static void CreateSphereCollider(const glm::vec3 center,
		float radius,
		glm::vec3 direction);
	static void CreateBoxCollider(const glm::vec3 position,
		const glm::vec3& size, 
		const glm::vec3& rotation, 
		glm::vec3 direction,
		bool movement = false);
};