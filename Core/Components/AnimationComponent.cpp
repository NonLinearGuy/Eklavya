#include "AnimationComponent.h"

#include "../Utils/Logger.h"

#include "../AssetManager/AssetManager.h"
#include "../AssetManager/Model.h"
#include "../AssetManager/Animation.h"
#include "../Animation/Joint.h"
#include <GLFW/glfw3.h>
#include "..//Debugging/Diagnostics.h"

ComponentID AnimationComponent::s_ID = 9;

AnimationComponent::AnimationComponent(const std::string & animationName)
	:
	m_AnimName(animationName)
	
{
	m_Transforms.reserve(100);

	for (int i = 0; i < 100; i++)
		m_Transforms.push_back(glm::mat4(1.0f));

	isSwitchingAnimation = false;
	elapsedSwitchTime = 0;
	maxSwitchedDuration = 5;
	switchSpeed = 0.5f;
	
}

AnimationComponent::~AnimationComponent()
{
	
}

void AnimationComponent::Init()
{
	auto idle = AssetManager::GetInstance().GetAsset<Animation>("Michelle/Idle");
	auto run1 = AssetManager::GetInstance().GetAsset<Animation>("Michelle/Running");
	auto run2 = AssetManager::GetInstance().GetAsset<Animation>("Michelle/Fast Run");
	m_Animations.push_back(idle);
	m_Animations.push_back(run1);
	m_Animations.push_back(run2);
	m_CurrentAnimation = m_Animations[0];
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
	m_DeltaTime = dt;

	if (!isSwitchingAnimation && m_CurrentAnimation)
	{
		m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
		m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
		CalculateJointTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
	}
	else
	{
		CalculateJointTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
	}
}

glm::mat4 AnimationComponent::InterpolateToNextAnimation(Joint* jointFrom, Joint* jointTo)
{
	glm::mat4 result(1.0f);
	elapsedSwitchTime += switchSpeed * m_DeltaTime;
	float scaleFactor =  elapsedSwitchTime / maxSwitchedDuration; // 0 - 1
	DiagManager::sGeneralDiagsMap[EMapKeys::KEY_DELTA] = std::to_string(elapsedSwitchTime);
	if (scaleFactor >= 1.0f)
	{
		isSwitchingAnimation = false;
		elapsedSwitchTime = 0;
		m_CurrentTime = 0;
		m_CurrentAnimation = m_NextAnimation;
		m_NextAnimation.reset();
	}
	else
		result = LerpPos(jointFrom,jointTo,scaleFactor) * SlerpRot(jointFrom,jointTo,scaleFactor) * LerpScale(jointFrom,jointTo,scaleFactor);
	return result;
}

glm::mat4 AnimationComponent::LerpPos(Joint* jointFrom, Joint* jointTo,float scaleFactor)
{
	int p0Index = jointFrom->GetPositionIndex(m_CurrentTime);
	int p1Index = jointTo->GetPositionIndex(0);
	glm::vec3 finalPos = glm::mix(jointFrom->GetPosAtIndex(p0Index), jointTo->GetPosAtIndex(p1Index)
		, scaleFactor);
	return glm::translate(glm::mat4(1.0), finalPos);
}

glm::mat4 AnimationComponent::SlerpRot(Joint* jointFrom, Joint* jointTo,float scaleFactor)
{
	int p0Index = jointFrom->GetRotationIndex(m_CurrentTime);
	int p1Index = jointTo->GetRotationIndex(0);
	glm::quat finalRotation = glm::slerp(jointFrom->GetRotAtIndex(p0Index), jointTo->GetRotAtIndex(p1Index)
		, scaleFactor);
	finalRotation = glm::normalize(finalRotation);
	return glm::toMat4(finalRotation);
}

glm::mat4 AnimationComponent::LerpScale(Joint* jointFrom, Joint* jointTo, float scaleFactor)
{
	int p0Index = jointFrom->GetScaleIndex(m_CurrentTime);
	int p1Index = jointTo->GetScaleIndex(0);
	glm::vec3 finalScale = glm::mix(jointFrom->GetScaleAtIndex(p0Index), jointTo->GetScaleAtIndex(p1Index)
		, scaleFactor);
	return glm::scale(glm::mat4(1.0),finalScale);
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

	if(joint)
	{
		if (isSwitchingAnimation && m_NextAnimation)
		{
			Joint* jointTo = m_NextAnimation->FindJoint(nodeName);
			nodeTransform = InterpolateToNextAnimation(joint,jointTo);
		}
		else
		{
			joint->Update(m_CurrentTime);
			nodeTransform = joint->GetLocalTransform();
		}
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


