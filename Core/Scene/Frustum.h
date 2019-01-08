#pragma once

#include "BaseNode.h"

class VertexArrayObject;
class IBoundVolume;

enum EPlane
{
	TOP_PLANE = 0,
	BOTTOM_PLANE,
	LEFT_PLANE,
	RIGHT_PLANE,
	FAR_PLANE,
	NEAR_PLANE
};


enum EPoint
{
	NEAR_CENTER,
	NEAR_TOP_LEFT,
	NEAR_TOP_RIGHT,
	NEAR_BOTTOM_LEFT,
	NEAR_BOTTOM_RIGHT,
	FAR_CENTER,
	FAR_TOP_LEFT,
	FAR_TOP_RIGHT,
	FAR_BOTTOM_LEFT,
	FAR_BOTTOM_RIGHT
};

using Plane = glm::vec4;

class Frustum
{
public:
	Frustum(float fov, float ratio, float fNear, float fFar);
	~Frustum();
	void UpdatePlanes(const glm::mat4& projection, const glm::mat4& view);
	VertexArrayObject* GetVAO() { return m_VAO; }
	void Destroy();
	
	bool IsInside(IBoundVolume* volume);
	void Draw();
private:
	
	bool IsSphereInside(const glm::vec3& center, float radius);
	
	Plane m_Planes[6];
	glm::vec3 m_Points[10];
	int m_Indices[36];
	float m_NearWidth, m_NearHeight, m_FarWidth, m_FarHeight;
	VertexArrayObject* m_VAO;
};