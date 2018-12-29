#pragma once

#include "GameObject.h"

class TPCamera 
{
public:
	TPCamera(std::shared_ptr<GameObject> pTarget);
	~TPCamera();
	void Update();
	glm::mat4 GetView();
	inline void SetOffset(const glm::vec3& offset) { m_Offset = offset; }
private:
	glm::vec3 m_Position;
	glm::vec3 m_Origin;
	glm::vec3 m_Up;
	glm::mat4 m_View;
	glm::vec3 m_Offset;
	std::shared_ptr<GameObject> m_Target;
};

