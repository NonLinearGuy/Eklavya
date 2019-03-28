#include "BoundingVolume.h"
#include "BaseNode.h"
#include "SphereNode.h"
#include "../Helpers.h"
#include "../Renderer/VertexArrayObject.h"
#include "../Renderer/Material.h"
#include "../Renderer/GLRenderer.h"
#include "../AssetManager/ShaderProgram.h"
#include "Scene.h"

IBoundVolume::IBoundVolume(EBoundType type) : m_Type(type)
{
}

SphereBound::SphereBound(const glm::vec3& center,float radius) 
	:
	IBoundVolume(EBoundType::SPHERE), 
	m_Center(center),
	m_Radius(radius)
{
}

SphereBound::~SphereBound()
{

}

BoxBound::BoxBound(const glm::vec3 & extents)
	:
	IBoundVolume(EBoundType::BOX),
	m_Transform(1.0f),
	m_Extents(extents)
{
	m_Points[0] = glm::vec4(-0.5f, -0.5f, 0.5f,1.0f);
	m_Points[1] = glm::vec4(0.5f, 0.5f, 0.5f,1.0f);
	m_Points[2] = glm::vec4(0.5f, -0.5f, 0.5f,1.0f);
	m_Points[3] = glm::vec4(-0.5f, 0.5f, 0.5f,1.0f);

	m_Points[4] = glm::vec4(-0.5f, -0.5f, -0.5f,1.0f);
	m_Points[5] = glm::vec4(0.5f, 0.5f, -0.5f,1.0f);
	m_Points[6] = glm::vec4(0.5f, -0.5f, -0.5f,1.0f);
	m_Points[7] = glm::vec4(-0.5f, 0.5f, -0.5f,1.0f);

}

BoxBound::~BoxBound()
{
}
