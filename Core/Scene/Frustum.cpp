#include "Frustum.h"
#include "../Renderer/VertexArrayObject.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include "../Helpers.h"
#include "BoundingVolume.h"


Frustum::Frustum(float fov, float ratio, float fNear, float fFar)
{
	float halfOfFov = fov / static_cast<float>(2);

	m_NearHeight = 2 * tan(glm::radians(halfOfFov)) * fNear;
	m_NearWidth = m_NearHeight * ratio;

	m_FarHeight = 2 * tan(glm::radians(halfOfFov)) * fFar;
	m_FarWidth = m_FarHeight * ratio;

	glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);

	glm::vec3 nc = m_Points[NEAR_CENTER] = forward * fNear; //assuming camera is at (0,0,0)
	glm::vec3 fc = m_Points[FAR_CENTER] = forward * fFar; //assuming camera is at (0,0,0)

	m_Points[NEAR_TOP_LEFT] = nc + (up * (m_NearHeight / 2.0f)) - (right * (m_NearWidth / 2.0f));
	m_Points[NEAR_TOP_RIGHT] = m_Points[NEAR_TOP_LEFT] + (right * m_NearWidth);
	m_Points[NEAR_BOTTOM_LEFT] = m_Points[NEAR_TOP_LEFT] - (up * m_NearHeight);
	m_Points[NEAR_BOTTOM_RIGHT] = m_Points[NEAR_BOTTOM_LEFT] + (right * m_NearWidth);

	m_Points[FAR_TOP_LEFT] = fc + (up * (m_FarHeight / 2.0f)) - (right * (m_FarWidth / 2.0f));
	m_Points[FAR_TOP_RIGHT] = m_Points[FAR_TOP_LEFT] + (right * m_FarWidth);
	m_Points[FAR_BOTTOM_LEFT] = m_Points[FAR_TOP_LEFT] - (up * m_FarHeight);
	m_Points[FAR_BOTTOM_RIGHT] = m_Points[FAR_BOTTOM_LEFT] + (right * m_FarWidth);

	glm::vec3 vertices[8];

	vertices[0] = m_Points[NEAR_BOTTOM_LEFT];
	vertices[1] = m_Points[NEAR_BOTTOM_RIGHT];
	vertices[2] = m_Points[NEAR_TOP_LEFT];
	vertices[3] = m_Points[NEAR_TOP_RIGHT];

	vertices[4] = m_Points[FAR_BOTTOM_LEFT];
	vertices[5] = m_Points[FAR_BOTTOM_RIGHT];
	vertices[6] = m_Points[FAR_TOP_LEFT];
	vertices[7] = m_Points[FAR_TOP_RIGHT];

	/* init_resources */
	int indices[] = {
		// front
		0, 1,
		0, 2,
		1, 3,
		2, 3,
		// back

		4, 5,
		4, 6,
		5, 7,
		6, 7,

		//middle
		0,4,
		1,5,
		2,6,
		3,7

	};

	m_VAO = new VertexArrayObject();
	m_VAO->Create(&vertices[0], sizeof(glm::vec3) * 8, &indices[0],sizeof(indices));
	m_VAO->SetPosPtr(3, 0, 0);
}

Frustum::~Frustum()
{
	
}

void Frustum::Destroy()
{
	m_VAO->Destroy();
}

void Frustum::UpdatePlanes(const glm::mat4& projection,const glm::mat4& view)
{
	glm::vec4 row1 = glm::row(projection, 0);
	glm::vec4 row2 = glm::row(projection, 1);
	glm::vec4 row3 = glm::row(projection, 2);
	glm::vec4 row4 = glm::row(projection, 3);
	// normals will be pointing inward
	m_Planes[EPlane::LEFT_PLANE] = glm::normalize(row4 + row1) * view ;
	m_Planes[EPlane::RIGHT_PLANE] = glm::normalize(row4 - row1) * view;
	m_Planes[EPlane::BOTTOM_PLANE] = glm::normalize(row4 + row2) * view;
	m_Planes[EPlane::TOP_PLANE] = glm::normalize(row4 - row2) * view;
	m_Planes[EPlane::NEAR_PLANE] = glm::normalize(row4 + row3) * view;
	m_Planes[EPlane::FAR_PLANE] = glm::normalize(row4 - row3) * view;
}

void Frustum::Draw()
{
	m_VAO->Bind();
	glDrawElements(GL_LINES,24,GL_UNSIGNED_INT,0);
}

bool Frustum::IsInside(IBoundVolume* volume)
{
	EBoundType type = volume->GetType();
	if (EBoundType::SPHERE == type)
	{
		SphereBound* sphere = static_cast<SphereBound*>(volume);
		return IsSphereInside(sphere->GetCenter(),sphere->GetRadius());
	}
	else
		return false;
}
bool Frustum::IsSphereInside(const glm::vec3& position, float radius)
{
	for (auto plane : m_Planes)
	{
		float dist = plane.x * position.x + plane.y * position.y + plane.z * position.z + plane.w;
		if (dist < -radius) return false;
	}
	return true;
}