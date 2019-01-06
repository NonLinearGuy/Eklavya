#pragma once

#include "BaseNode.h"
#include <glad/glad.h>

//frustum at origin facing (0,0,-1)

class VertexArrayObject;
class ShaderProgram;

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

class FrustumNode : public BaseNode
{
public:
	FrustumNode(float fov, float ratio, float near, float far);
	~FrustumNode();
	void PreRender(Scene* scene)override;
	void Render(Scene *scene)override;
	void PostRender(Scene* scene)override;
	bool SphereInsideFrustum(glm::vec3 position, float radius);
	void CreatePlane(glm::mat4 projection);
	
private:
	glm::mat4 m_ToWorld;
	
	Plane planes[6];

	glm::vec3 m_Points[10];
	glm::vec3 m_Vertices[8];
	GLuint m_Indices[36];
	float m_NearWidth, m_NearHeight, m_FarWidth, m_FarHeight;
	VertexArrayObject* m_VAO;
	ShaderProgram* m_Shader;
};