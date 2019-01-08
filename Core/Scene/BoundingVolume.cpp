#include "BoundingVolume.h"
#include "BaseNode.h"
#include "SphereNode.h"
#include "../Helpers.h"

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
