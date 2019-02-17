#ifndef _INC_CONTACT_DATA_H_
#define _INC_CONTACT_DATA_H_

#include <glm/glm.hpp>
#include <vector>
#include <memory>

class BoxCollider;
class SphereCollider;
class RigidBodyComponent;

struct ContactData
{
public:
	glm::vec3 m_Point;
	glm::vec3 m_Normal;
	float	  m_PenetrationDepth;
	std::shared_ptr<RigidBodyComponent> m_BodyA;
	std::shared_ptr<RigidBodyComponent> m_BodyB;
	glm::vec3 m_DesiredDeltaVel;
	/*holds relative position from center of both bodies*/
	glm::vec3 m_RelContactPositions[2];
	glm::mat3 m_ContactToWorld;
	void CalculateContactToWorld();
};


class IntersectionTests
{
public:
	//plane - box
	//plane - sphere
	//sphere - box
	//sphere - sphere
	//box - box
	static bool BoxAndBox(std::shared_ptr<BoxCollider> boxFirst, std::shared_ptr<BoxCollider> boxSecond);
	static bool BoxAndSphere(std::shared_ptr<BoxCollider> box, std::shared_ptr<SphereCollider> sphere);
	static bool SphereAndSphere(std::shared_ptr<SphereCollider> sphereFirst, std::shared_ptr<SphereCollider> sphereSecond);
private:
	static bool OverlapOnAxis(std::shared_ptr<BoxCollider> one,std::shared_ptr<BoxCollider> two,glm::vec3 axis);
	static float GetProjectedLength(std::shared_ptr<BoxCollider> box, glm::vec3 axis);
};

class ContactGenerator
{
public:
	
	static bool SphereAndSphere(std::shared_ptr<SphereCollider> sphereOne,
		std::shared_ptr<SphereCollider> sphereTwo,
		std::vector<ContactData>& pContacts
	);

	static bool SphereAndBox(std::shared_ptr<BoxCollider> box,
		std::shared_ptr<SphereCollider> sphere,
		std::vector<ContactData>& pContacts
	);
	
};

#endif