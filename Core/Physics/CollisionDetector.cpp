#include "CollisionDetector.h"
#include "Collider.h"
#include "../Components/RigidBodyComponent.h"
#include "../Debugging/Diagnostics.h"
#include <sstream>

//INTERSECTION TESTS

bool IntersectionTests::OverlapOnAxis(std::shared_ptr<BoxCollider> one, std::shared_ptr<BoxCollider> two, glm::vec3 axis)
{
	if (glm::length2(axis) < .0001f) return true;

	glm::vec3 box1Pos = one->GetBody()->GetAxis(3);
	glm::vec3 box2Pos = two->GetBody()->GetAxis(3);
	glm::vec3 toCenter = box2Pos - box1Pos;

	float projectedMidlineLen = abs(glm::dot(toCenter,axis));
	float projectedBoxOneLen = GetProjectedLength(one, axis);
	float projectedBoxTwoLen = GetProjectedLength(two, axis);
	return projectedMidlineLen < (projectedBoxOneLen + projectedBoxTwoLen);
}

float IntersectionTests::GetProjectedLength(std::shared_ptr<BoxCollider> box, glm::vec3 axis)
{
	auto body = box->GetBody();
	glm::vec3 scale = box->GetHalfSize(); //considering box has uniform haflsize for all axes
	float x = scale.x * glm::abs(glm::dot(body->GetAxis(0), axis));
	float y = scale.y * glm::abs(glm::dot(body->GetAxis(1), axis));
	float z = scale.z * glm::abs(glm::dot(body->GetAxis(2), axis));
	float length = (x + y + z);
	return length;
}

bool IntersectionTests::BoxAndBox(std::shared_ptr<BoxCollider> one, std::shared_ptr<BoxCollider> two)
{
	bool collision =
		OverlapOnAxis(one, two, one->GetBody()->GetAxis(0)) &&
		OverlapOnAxis(one, two, one->GetBody()->GetAxis(1)) &&
		OverlapOnAxis(one, two, one->GetBody()->GetAxis(2)) &&

		OverlapOnAxis(one, two, two->GetBody()->GetAxis(0)) &&
		OverlapOnAxis(one, two, two->GetBody()->GetAxis(1)) &&
		OverlapOnAxis(one, two, two->GetBody()->GetAxis(2)) &&

		OverlapOnAxis(one, two, glm::cross(one->GetBody()->GetAxis(0), two->GetBody()->GetAxis(0))) &&
		OverlapOnAxis(one, two, glm::cross(one->GetBody()->GetAxis(0), two->GetBody()->GetAxis(1))) &&
		OverlapOnAxis(one, two, glm::cross(one->GetBody()->GetAxis(0), two->GetBody()->GetAxis(2))) &&

		OverlapOnAxis(one, two, glm::cross(one->GetBody()->GetAxis(1), two->GetBody()->GetAxis(0))) &&
		OverlapOnAxis(one, two, glm::cross(one->GetBody()->GetAxis(1), two->GetBody()->GetAxis(1))) &&
		OverlapOnAxis(one, two, glm::cross(one->GetBody()->GetAxis(1), two->GetBody()->GetAxis(2))) &&
		
		OverlapOnAxis(one, two, glm::cross(one->GetBody()->GetAxis(2), two->GetBody()->GetAxis(0))) &&
		OverlapOnAxis(one, two, glm::cross(one->GetBody()->GetAxis(2), two->GetBody()->GetAxis(1))) &&
		OverlapOnAxis(one, two, glm::cross(one->GetBody()->GetAxis(2), two->GetBody()->GetAxis(2)));

	return collision;
		
}
bool IntersectionTests::BoxAndSphere(std::shared_ptr<BoxCollider>  box, std::shared_ptr<SphereCollider> sphere)
{
	glm::vec3 localSpherePosition = box->GetBody()->GetPointInLocalSpace(sphere->GetBody()->GetPosition());
	
	bool xIsGreater = abs(localSpherePosition.x) - sphere->GetRadius() > box->GetHalfSize().x;
	bool yIsGreater = abs(localSpherePosition.y) - sphere->GetRadius() > box->GetHalfSize().y;
	bool zIsGreater = abs(localSpherePosition.z) - sphere->GetRadius() > box->GetHalfSize().z;

	if (!xIsGreater && !yIsGreater && !zIsGreater) return true;
	return false;

}

bool IntersectionTests::SphereAndSphere(std::shared_ptr<SphereCollider> one, std::shared_ptr<SphereCollider> two)
{
	glm::vec3 midline = one->GetBody()->GetPosition() - two->GetBody()->GetPosition();
	float totalRadius = one->GetRadius() + two->GetRadius();
	return (midline.x * midline.x) + (midline.y * midline.y) + (midline.z * midline.z) < totalRadius * totalRadius;
}



//CONTACT GENERATION


bool ContactGenerator::SphereAndSphere(std::shared_ptr<SphereCollider> sphereOne, std::shared_ptr<SphereCollider> sphereTwo, std::vector<ContactData>& pContacts)
{
	if (!IntersectionTests::SphereAndSphere(sphereOne, sphereTwo)) return false;

	ContactData newContact;
	newContact.m_BodyA = sphereOne->GetBody();
	newContact.m_BodyB = sphereTwo->GetBody();

	glm::vec3 p1 = newContact.m_BodyA->GetAxis(3);
	glm::vec3 p2 = newContact.m_BodyB->GetAxis(3);

	glm::vec3 toSphereTwo = p2 - p1;
	float size = glm::length(toSphereTwo);

	newContact.m_Normal = toSphereTwo / size;
	newContact.m_Point = p1 + (toSphereTwo / 2.0f);
	newContact.m_PenetrationDepth = (sphereOne->GetRadius() + sphereTwo->GetRadius()) - size;

	std::stringstream ss;
	ss << "("<<newContact.m_Point.x<<"," << newContact.m_Point.y << ","<<newContact.m_Point.z << ")";
	DiagManager::sPhysicsDiagsMap[KEY_CONTACT_POINT] = ss.str();
	ss.str("");
	ss << "(" << newContact.m_Normal.x << "," << newContact.m_Normal.y << "," << newContact.m_Normal.z << ")";
	DiagManager::sPhysicsDiagsMap[KEY_NORMAL] = ss.str();
	DiagManager::sPhysicsDiagsMap[KEY_INTERPEN] = std::to_string(newContact.m_PenetrationDepth);
	pContacts.push_back(newContact);

	return true;
}



bool ContactGenerator::SphereAndBox(std::shared_ptr<BoxCollider> box, std::shared_ptr<SphereCollider> sphere, std::vector<ContactData>& pContacts)
{
	//early test
	if (!IntersectionTests::BoxAndSphere(box, sphere))
		return false;

	ContactData newContact;
	newContact.m_BodyA = sphere->GetBody();
	newContact.m_BodyA = box->GetBody();

	glm::vec3 relativePos = box->GetBody()->GetPointInLocalSpace(sphere->GetBody()->GetAxis(3));
	glm::vec3 halfSize = box->GetHalfSize();
	glm::vec3 closestPoint = glm::clamp(relativePos,-1.0f * halfSize,halfSize);

	glm::vec3  closestPointWorld = box->GetBody()->GetPointInWorldSpace(closestPoint);

	newContact.m_Point = closestPointWorld;
	newContact.m_Normal = glm::normalize(sphere->GetBody()->GetAxis(3) - closestPointWorld);
	newContact.m_PenetrationDepth = sphere->GetRadius() - glm::length(closestPoint - relativePos);

	pContacts.push_back(newContact);

	std::stringstream ss;
	ss << "(" << newContact.m_Point.x << "," << newContact.m_Point.y << "," << newContact.m_Point.z << ")";
	DiagManager::sPhysicsDiagsMap[KEY_CONTACT_POINT] = ss.str();
	ss.str("");
	ss << "(" << newContact.m_Normal.x << "," << newContact.m_Normal.y << "," << newContact.m_Normal.z << ")";
	DiagManager::sPhysicsDiagsMap[KEY_NORMAL] = ss.str();
	DiagManager::sPhysicsDiagsMap[KEY_INTERPEN] = std::to_string(newContact.m_PenetrationDepth);
	pContacts.push_back(newContact);

	return true;

}

bool ContactGenerator::BoxAndBox(std::shared_ptr<BoxCollider> box1, std::shared_ptr<BoxCollider> box2, std::vector<ContactData>& pContacts)
{
	
	return false;
}

bool ContactGenerator::BoxAndPoint(std::shared_ptr<BoxCollider> box1, const glm::vec3 vertex, std::vector<ContactData>& pContacts)
{
	return false;
}
