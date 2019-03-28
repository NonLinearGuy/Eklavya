#include "Animator.h"
#include "Joint.h"
#include "../Model.h"
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


void Animator::CalculateJointTransform(const aiNode* node, glm::mat4 parentTransform)
{
	std::string nodeName = node->mName.data;
	glm::mat4 nodeTransform = GetGLMMat(node->mTransformation);
	Joint* joint = m_CurrentAnimation->FindJoint(node->mName.data);
	if (joint)
	{
		joint->Update(m_CurrentTime);
		nodeTransform = joint->GetLocalTransform();
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransform;
	auto boneIDMap = m_CurrentAnimation->GetBoneIDMap();
	if (boneIDMap.find(nodeName) != boneIDMap.end())
	{
		int index = boneIDMap[nodeName];
		auto boneMap = m_CurrentAnimation->GetOffsetMap();
		glm::mat4 offset = boneMap[nodeName];
		m_FinalTransforms[index] = m_CurrentAnimation->GetGlobalInverse() * globalTransformation * offset;
	}


	for (int i = 0; i < node->mNumChildren; i++)
		CalculateJointTransform(node->mChildren[i], globalTransformation);

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







