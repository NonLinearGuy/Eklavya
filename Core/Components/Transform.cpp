#include "Transform.h"


ComponentID Transform::s_ID = 0;

Transform::Transform(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& pEulerAngles)
	:
	BaseComponent(),
	m_Position(position),
	m_Rotation(pEulerAngles),
	m_Scale(scale)
{
}

Transform::~Transform()
{
}


void Transform::SetRotation(const glm::vec3 & pEulerAngles)
{
	m_Rotation = glm::quat(pEulerAngles);
}

void Transform::SetRotation(const glm::quat & pRotation)
{
	m_Rotation = pRotation;
}

glm::vec3 Transform::TransformToLocal(const glm::vec3 & vertex)
{
	glm::mat4 rotation = glm::toMat4(m_Rotation);
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_Position);
	return  glm::inverse(translation * rotation) * glm::vec4(vertex, 1.0f);
}

glm::mat4 Transform::GetModelMatrix()
{
	glm::mat4 rotation = glm::toMat4(m_Rotation);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f),m_Scale);
	glm::mat4 translation = glm::translate(glm::mat4(1.0f),m_Position);

	return translation * rotation * scale;
}
