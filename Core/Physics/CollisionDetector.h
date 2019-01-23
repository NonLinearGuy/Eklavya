#ifndef _INC_CONTACT_DATA_H_
#define _INC_CONTACT_DATA_H_

#include <glm/glm.hpp>
#include <vector>
#include <memory>

class BoxCollider;
class SphereCollider;

struct ContactData
{
public:
	glm::vec3 m_Point;
	glm::vec3 m_Normal;
	float	  m_PenetrationDepth;
};


class CollisionData
{
public:
	//bool  Generate(const ICollider& one, const ICollider& two);
private:
	std::vector<ContactData> m_Contacts;
};



class CollisionDetector
{
public:
	static bool BoxAndSphere(std::shared_ptr<BoxCollider> one, std::shared_ptr<SphereCollider> two);
};

#endif