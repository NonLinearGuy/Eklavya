#pragma once

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <assimp/scene.h>

class KeyFrame
{
public:
	KeyFrame();
	KeyFrame(const aiQuaternion& orientation,const aiVector3D& position, const aiVector3D& scale,float timeStamp);
	~KeyFrame();
	glm::quat m_Orientation;
	glm::vec3 m_Position;
	glm::vec3 m_Scale;
	float m_TimeStamp;
};