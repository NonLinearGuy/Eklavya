#include "Transform.h"


Transform::Transform(const glm::vec3 & pPosition, const glm::vec3 & pEulerAngles, const glm::vec3 & pScale)
	:
	BaseComponent("Transform"),
	m_Position(pPosition),
	m_Rotation(pEulerAngles),
	m_Scale(pScale)
{
}

Transform::~Transform()
{
}


void Transform::SetRotation(const glm::vec3 & pEulerAngles)
{
	m_Rotation = glm::quat(pEulerAngles);
}

glm::mat4 Transform::GetModelMatrix()
{
	glm::mat4 rotation = glm::toMat4(m_Rotation);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f),m_Scale);
	glm::mat4 translation = glm::translate(glm::mat4(1.0f),m_Position);

	return translation * rotation * scale;
}
