#include "AnimationComponent.h"

#include "../Utils/Logger.h"

#include "../AssetManager/AssetManager.h"
#include "../AssetManager/Model.h"
#include "../AssetManager/Animation.h"
#include "../Animation/Animator.h"
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
	m_Animation = AssetManager::GetInstance().GetAsset<Animation>(m_AnimName);
	m_Animator = std::make_shared<Animator>();
	m_Animator->PlayAnimation(m_Animation);
}

void AnimationComponent::Destroy()
{
	
}

void AnimationComponent::OnKeyAction(int key, int action)
{
	
}


void AnimationComponent::Tick(float dt)
{
	m_Animator->Tick(dt);

	m_Transforms.clear();
	m_Transforms.resize(100);
	auto jointTransformMap = m_Animator->GetFinalTransform();
	for (auto pair : jointTransformMap)
		m_Transforms[pair.first] = pair.second;
}

