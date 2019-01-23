#include "CollisionDetector.h"
#include "Collider.h"

bool CollisionDetector::BoxAndSphere(std::shared_ptr<BoxCollider>  box, std::shared_ptr<SphereCollider> sphere)
{
	glm::vec3 localSpherePosition = box->TransformToLocal(sphere->GetPosition());
	
	bool xIsGreater = abs(localSpherePosition.x) - sphere->GetRadius() > box->GetHalfSize().x;
	bool yIsGreater = abs(localSpherePosition.y) - sphere->GetRadius() > box->GetHalfSize().y;
	bool zIsGreater = abs(localSpherePosition.z) - sphere->GetRadius() > box->GetHalfSize().z;

	if (!xIsGreater && !yIsGreater && !zIsGreater) return true;
	return false;

}
