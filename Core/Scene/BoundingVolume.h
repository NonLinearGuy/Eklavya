#ifndef _INC_BOUNDING_VOLUME_H_
#define _INC_BOUNDING_VOLUME_H_

#include <memory>
#include <glm/glm.hpp>
#include "BaseNode.h"


class Scene;
class SphereVAO;

enum class EBoundType
{
	SPHERE = 0,
	BOX
};

class IBoundVolume
{
public:
	IBoundVolume(EBoundType type);
	virtual EBoundType GetType() { return m_Type; }
protected:
	EBoundType m_Type;
	//for debugging
	std::shared_ptr<BaseNode> m_BaseNode;
};

class SphereBound : public IBoundVolume
{
public:
	SphereBound(const glm::vec3& center,float radius);
	~SphereBound();
	inline void SetCenter(const glm::vec3& center) { m_Center = center; }
	inline void SetRadius(float m_Radius) {}
	inline glm::vec3 GetCenter() { return m_Center; }
	inline float GetRadius() { return m_Radius; }

private:
	float m_Radius;
	glm::vec3 m_Center;
};

class BoxBound : public IBoundVolume
{
public:
	BoxBound(const glm::vec3& extents);
	~BoxBound();
	inline void SetTransform(const glm::mat4& transform) { m_Transform = transform; }
	inline void SetExtents(const glm::vec3& extents) { m_Extents = extents; }
	inline glm::vec3 GetExtents() { return m_Extents; }
	inline glm::vec4* GetPoints() { return m_Points; }
	inline glm::mat4 GetTransform() { return m_Transform; }
private:
	glm::mat4 m_Transform;
	glm::vec3 m_Extents;
	glm::vec4 m_Points[8];
};

#endif