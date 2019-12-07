#include "AnimationComponent.h"

#include "../Utils/Logger.h"

#include "../AssetManager/AssetManager.h"
#include "../AssetManager/Model.h"
#include "../AssetManager/Animation.h"
#include "../Animation/Joint.h"
#include <GLFW/glfw3.h>

ComponentID AnimationComponent::s_ID = 9;

AnimationComponent::AnimationComponent(const std::string & animationName)
	:
	m_AnimName(animationName)
	
{
	m_Transforms.reserve(100);

	for (int i = 0; i < 100; i++)
		m_Transforms.push_back(glm::mat4(1.0f));

	
}

AnimationComponent::~AnimationComponent()
{
	
}

void AnimationComponent::Init()
{
	m_CurrentAnimation = AssetManager::GetInstance().GetAsset<Animation>(m_AnimName);
	m_CurrentTime = 0.0f;
}


void AnimationComponent::Destroy()
{
	
}

void AnimationComponent::OnKeyAction(int key, int action)
{
	
}


void AnimationComponent::Tick(float dt)
{
	if (m_CurrentAnimation)
	{
		m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
		m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
		CalculateJointTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
	}
}


void AnimationComponent::PlayAnimation(std::shared_ptr<Animation> pAnimation)
{
	m_CurrentAnimation = pAnimation;
	m_CurrentTime = 0.0f;
}


void AnimationComponent::CalculateJointTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
	std::string nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;
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
		m_Transforms[index] = globalTransformation * offset;
	}

	for (int i = 0; i < node->childrenCount; i++)
		CalculateJointTransform(&node->children[i], globalTransformation);
}


