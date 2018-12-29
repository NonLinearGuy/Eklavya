#include "ThirdPersonCamera.h"
#include <glm/gtc/matrix_transform.hpp>

TPCamera::TPCamera(std::shared_ptr<GameObject> pTarget) : m_Target(pTarget), m_Position(0.0f),m_Origin(0.0f), m_Up(0.0f,1.0f,0.0f)
{
}


TPCamera::~TPCamera()
{

}

void TPCamera::Update()
{
	
}

glm::mat4 TPCamera::GetView()
{
	glm::mat4 model = m_Target->GetWorldMatrix();
	m_Position = glm::vec3(model * glm::vec4(m_Target->GetTransform().GetPosition(), 1.0f));
	m_Offset = glm::vec3(glm::vec4(m_Offset,1.0f) * model);
	m_Up = glm::vec3(glm::vec4(m_Up, 1.0f) * model);
	return glm::lookAt(m_Position + m_Offset, m_Position, glm::vec3(0.0f,1.0f,0.0f));
}