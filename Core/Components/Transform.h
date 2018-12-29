#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "BaseComponent.h"

class Transform : public BaseComponent
{
public:
	Transform(const glm::vec3& pPosition = glm::vec3(0.0f),const glm::vec3& pEulerAngles = glm::vec3(0.0f),const glm::vec3& pScale = glm::vec3(1.0f));
	~Transform();
	void SetRotation(const glm::vec3& pNewYawPitchRoll);

	inline void SetPosition(const glm::vec3& pNewPosition) { m_Position = pNewPosition; }
	inline void SetX(float pX) { m_Position.x = pX; }
	inline void SetY(float pY) { m_Position.y = pY; }
	inline void SetZ(float pZ) { m_Position.z = pZ; }
	inline void SetScale(const glm::vec3& pNewScale) { m_Scale = pNewScale; }
	inline void SetScaleX(float pX) { m_Scale.x = pX; }
	inline void SetScaleY(float pY) { m_Scale.y = pY; }
	inline void SetScaleZ(float pZ) { m_Scale.z = pZ; }

	inline const glm::quat& GetRotation() { return m_Rotation; }
	inline const glm::vec3& GetPosition() const { return m_Position; }
	inline const glm::vec3& GetScale()const { return m_Scale; }

	glm::mat4 GetModelMatrix();

private:
	
	glm::vec3 m_Model;
	glm::vec3 m_Position;
	glm::quat m_Rotation;
	glm::vec3 m_Scale;
};

