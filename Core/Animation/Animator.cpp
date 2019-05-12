#include "Animator.h"
#include "Joint.h"
#include "../AssetManager/Model.h"
#include <glm/gtc/type_ptr.hpp>
#include "../Debugging/Diagnostics.h"
#include "../Helpers.h"

Animator::Animator()
	:
	m_CurrentAnimation(nullptr)
{

}

Animator::~Animator()
{
}

void Animator::PlayAnimation(std::shared_ptr<Animation> pAnimation)
{
	m_CurrentAnimation = pAnimation;
	m_CurrentTime = 0.0f;
}


void Animator::CalculateJointTransform(const AssimpNodeData& node, glm::mat4 parentTransform)
{
	std::string nodeName = node.name;
	glm::mat4 nodeTransform = node.transformation;
	Joint* joint = m_CurrentAnimation->FindJoint(nodeName);
	if (joint)
	{
		joint->Update(m_CurrentTime);
		nodeTransform = joint->GetLocalTransform();
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransform;
	auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
	if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	{
		int index = boneInfoMap[nodeName].id;
		glm::mat4 offset = boneInfoMap[nodeName].offset;
		m_FinalTransforms[index] = globalTransformation * offset;
	}

	for (int i = 0; i < node.childrenCount; i++)
		CalculateJointTransform(node.children[i], globalTransformation);
}

void Animator::Tick(float delta)
{
	if (m_CurrentAnimation)
	{
		m_FinalTransforms.clear();
		m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * delta;
		m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
		CalculateJointTransform(m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
	}
}







