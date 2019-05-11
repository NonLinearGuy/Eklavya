#include "RigidBodyComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../Physics/Collider.h"
#include "GameActor.h"
#include "Transform.h"
#include "../Helpers.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

unsigned int RigidBodyComponent::s_ID = 11;

RigidBodyComponent::RigidBodyComponent(std::shared_ptr<ICollider> pCollider)
	:
	m_Collider(pCollider),
	m_IsAwake(false),
	m_InverseMass(1.0f),
	m_Forces(0.0f),
	m_Torques(0.0f, 0.0f, 0.0f),
	//Linear
	m_Position(0.0f, 0.0f, 0.0f),
	m_LinearVelocity(0.0f, 0.0f, 0.0f),
	m_LinearAcceleration(0.0f, 0.0f, 0.0f),
	m_LinearDamping(.99f),
	//angular
	m_AngularAcceleration(0.0f),
	m_Orientation(glm::vec3(0.0f)),
	m_Rotation(0.0f),
	m_AngularDamping(.99f),
	//matrices
	m_InverseTensor(1.0f),
	m_InverseTensorWorld(1.0f),
	m_TransformMatrix(1.0f),

	m_Restitution(0.0f), // no bounce by default
	m_BodyType(DYNAMIC_BODY)

{
}

RigidBodyComponent::~RigidBodyComponent()
{
}

void RigidBodyComponent::Init()
{
}

void RigidBodyComponent::Destroy()
{
}


void RigidBodyComponent::UpdateData()
{
	m_Orientation = glm::normalize(m_Orientation);

	glm::mat4 orientation = glm::toMat4(m_Orientation);
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_Position);
	m_TransformMatrix = translation * orientation;

	//Recalculating inverse tensor
	m_InverseTensorWorld = glm::mat3(orientation) * m_InverseTensor;

	//updating matrices for rendering
	auto transform = MakeSharedPtr(m_Owner->GetComponent<Transform>(Transform::s_ID));
	transform->SetPosition(m_Position);
	transform->SetRotation(m_Orientation);
}

glm::vec3 RigidBodyComponent::GetPointInLocalSpace(const glm::vec3& point)
{
	return glm::inverse(m_TransformMatrix) * glm::vec4(point,1.0f);
}

glm::vec3 RigidBodyComponent::GetPointInWorldSpace(const glm::vec3& point)
{
	return m_TransformMatrix * glm::vec4(point, 1.0f);
}

glm::vec3 RigidBodyComponent::GetAxis(int index)
{
	return m_TransformMatrix[index];
}

void RigidBodyComponent::Tick(float deltaTime)
{
	if (!m_IsAwake || EBodyType::STATIC_BODY == m_BodyType) {
		UpdateData();
		return;
	}

	float delta = .033f;;

	glm::vec3 acc = m_LinearAcceleration;
	acc += m_InverseMass * m_Forces;
	m_LinearVelocity += acc * delta;
	
	glm::vec3 angularAcc = m_AngularAcceleration;// +m_InverseTensorWorld * m_Torques;
	m_Rotation += angularAcc * delta;

	//Damping 
	m_LinearVelocity *= pow(m_LinearDamping, 2.0f);
	//m_Rotation *= pow(m_AngularDamping,2.0f);

	m_Position += m_LinearVelocity * delta;

	glm::quat w(m_Rotation * delta);
	w.w = 0.0f;
	w *= m_Orientation;
	m_Orientation += (w * 0.5f);

	UpdateData();

	m_Forces = glm::vec3(0.0f);
	m_Torques = glm::vec3(0.0f);
}

void RigidBodyComponent::SetMass(float mass)
{
	assert(mass > 0.0f);
	m_Mass = mass;
	m_InverseMass = 1.0f / mass;
	UpdateTensor();
}

void RigidBodyComponent::SetInverseMass(float inverseMass)
{
	m_InverseMass = inverseMass;
	if(m_InverseMass >= .00000001f)
		m_Mass = 1.0f / m_InverseMass;
	else
	{
		m_Mass = 10000000000.0f;
	}
	UpdateTensor();
}


void RigidBodyComponent::UpdateTensor()
{
	EColliderType type = m_Collider->GetType();

	switch (type)
	{
	case EColliderType::SPHERE:
		SetTensorForSphere();
		break;
	case EColliderType::BOX:
		SetTensorForCuboid();
		break;
	default:
		SetTensorIdentity();
		break;
	};
}


void RigidBodyComponent::SetTensorForSphere()
{
	glm::mat4 tensor(1.0f);
	float radius = std::static_pointer_cast<SphereCollider>(m_Collider)->GetRadius();
	float scalar = (2.0f / 5.0f) * m_Mass * radius * radius;
	tensor[0][0] = tensor[1][1] = tensor[2][2] = scalar;
	m_InverseTensor = glm::inverse(tensor);
}

void RigidBodyComponent::SetTensorForCuboid()
{
	glm::mat4 tensor(1.0f);
	float scalar = ( 1.0f / 12.0f ) * m_Mass;
	glm::vec3 halfSize = std::static_pointer_cast<BoxCollider>(m_Collider)->GetHalfSize();

	tensor[0][0] = scalar * (halfSize.y * halfSize.y + halfSize.z * halfSize.z);
	tensor[1][1] = scalar * (halfSize.x * halfSize.x + halfSize.z * halfSize.z);
	tensor[2][2] = scalar * (halfSize.x * halfSize.x + halfSize.y * halfSize.y);

	m_InverseTensor = glm::inverse(tensor);
}

void RigidBodyComponent::SetTensorIdentity()
{
	m_InverseTensor = glm::mat4(1.0f);
}


