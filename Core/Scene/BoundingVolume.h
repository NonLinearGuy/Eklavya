#ifndef _INC_BOUNDING_VOLUME_H_
#define _INC_BOUNDING_VOLUME_H_

#include <memory>
#include <glm/glm.hpp>

class BaseNode;

enum class EBoundType
{
	SPHERE = 0
};

class IBoundVolume 
{
public:
	IBoundVolume(EBoundType type);
	virtual EBoundType GetType() { return m_Type; }
protected:
	EBoundType m_Type;
};

class SphereBound : public IBoundVolume
{
public:
	SphereBound(const glm::vec3& center,float radius);
	inline void SetCenter(const glm::vec3& center) { m_Center = center; }
	inline void SetRadius(float m_Radius) {}
	inline glm::vec3 GetCenter() { return m_Center; }
	inline float GetRadius() { return m_Radius; }
private:
	float m_Radius;
	glm::vec3 m_Center;
};

#endif