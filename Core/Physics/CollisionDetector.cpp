#include "CollisionDetector.h"
#include "Collider.h"
#include "../Components/RigidBodyComponent.h"



bool CollisionDetector::OverlapOnAxis(std::shared_ptr<BoxCollider> one, std::shared_ptr<BoxCollider> two, glm::vec3 axis)
{
	glm::vec3 box1Pos = one->GetBody()->GetAxis(3);
	glm::vec3 box2Pos = two->GetBody()->GetAxis(3);
	glm::vec3 toCenter = box2Pos - box1Pos;

	float projectedMidlineLen = abs(glm::dot(toCenter,axis));
	float projectedBoxOneLen = GetTransformedLength(one, axis);
	float projectedBoxTwoLen = GetTransformedLength(two, axis);
	return projectedMidlineLen < (projectedBoxOneLen + projectedBoxTwoLen);
}

float CollisionDetector::GetTransformedLength(std::shared_ptr<BoxCollider> box, glm::vec3 axis)
{
	auto body = box->GetBody();
	glm::vec3 scale = box->GetHalfSize(); //considering box has uniform haflsize for all axes
	float x = scale.x * glm::abs(glm::dot(body->GetAxis(0), axis));
	float y = scale.y * glm::abs(glm::dot(body->GetAxis(1), axis));
	float z = scale.z * glm::abs(glm::dot(body->GetAxis(2), axis));
	float length = (x + y + z);
	return length;
}

bool CollisionDetector::BoxAndBox(std::shared_ptr<BoxCollider> one, std::shared_ptr<BoxCollider> two)
{
	bool collision =
		OverlapOnAxis(one, two, one->GetBody()->GetAxis(0)) &&
		OverlapOnAxis(one, two, one->GetBody()->GetAxis(1)) &&
		OverlapOnAxis(one, two, one->GetBody()->GetAxis(2)) &&

		OverlapOnAxis(one, two, two->GetBody()->GetAxis(0)) &&
		OverlapOnAxis(one, two, two->GetBody()->GetAxis(1)) &&
		OverlapOnAxis(one, two, two->GetBody()->GetAxis(2));
	return collision;
}
bool CollisionDetector::BoxAndSphere(std::shared_ptr<BoxCollider>  box, std::shared_ptr<SphereCollider> sphere)
{
	glm::vec3 localSpherePosition = box->GetBody()->GetPointInLocalSpace(sphere->GetBody()->GetPosition());
	
	bool xIsGreater = abs(localSpherePosition.x) - sphere->GetRadius() > box->GetHalfSize().x;
	bool yIsGreater = abs(localSpherePosition.y) - sphere->GetRadius() > box->GetHalfSize().y;
	bool zIsGreater = abs(localSpherePosition.z) - sphere->GetRadius() > box->GetHalfSize().z;

	if (!xIsGreater && !yIsGreater && !zIsGreater) return true;
	return false;

}

bool CollisionDetector::SphereAndSphere(std::shared_ptr<SphereCollider> one, std::shared_ptr<SphereCollider> two)
{
	glm::vec3 midline = one->GetBody()->GetPosition() - two->GetBody()->GetPosition();
	float totalRadius = one->GetRadius() + two->GetRadius();
	return (midline.x * midline.x) + (midline.y * midline.y) + (midline.z * midline.z) < totalRadius * totalRadius;
}



