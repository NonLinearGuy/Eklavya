#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "BaseComponent.h"

class Transform : public BaseComponent
{
public:
	Transform(const glm::vec3& position = glm::vec3(0.0f),const glm::vec3& scale = glm::vec3(1.0f),const glm::vec3& pEulerAngles = glm::vec3(0.0f));
	~Transform();
	
	void SetRotation(const glm::quat& pRotation);
	void SetRotation(const glm::vec3& pNewYawPitchRoll);
	
	inline void SetPosition(float x, float y, float z) { m_Position = glm::vec3(x, y, z); }
	inline void SetPosition(const glm::vec3& pNewPosition) { m_Position = pNewPosition; }
	
	inline void SetScale(float x,float y,float z) { m_Scale = glm::vec3(x,y,z); }
	inline void SetScale(float uniformScale) { m_Scale = glm::vec3(uniformScale); }
	inline void SetScale(glm::vec3 scale) { m_Scale = scale; }

	inline const glm::quat& GetRotation() { return m_Rotation; }
	inline const glm::vec3& GetPosition() const { return m_Position; }
	inline const glm::vec3& GetScale()const { return m_Scale; }

	glm::vec3 TransformToLocal(const glm::vec3& vertex);
	glm::vec3 TransformToWorld(const glm::vec3& vertex);

	glm::mat4 GetTRMatrix();
	glm::mat4 GetModelMatrix();
	glm::mat4 GetRotationMatrix();

	ComponentID GetID()override { return s_ID; }
	static ComponentID s_ID;
private:
	glm::vec3 m_Position;
	glm::quat m_Rotation;
	glm::vec3 m_Scale;
	glm::mat4 m_Model;
};

