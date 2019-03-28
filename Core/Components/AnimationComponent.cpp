#include "AnimationComponent.h"

#include "../Utils/Logger.h"
#include <assimp/postprocess.h>
#include "../Model.h"
#include "../Animation/Animation.h"
#include "../Animation/Animator.h"

ComponentID AnimationComponent::s_ID = 9;

AnimationComponent::AnimationComponent(const std::string & animationName)
	:
	m_AnimName(animationName)
{
	m_Transforms.reserve(80);

	for (int i = 0; i < 80; i++)
		m_Transforms.push_back(glm::mat4(1.0f));

	
}

AnimationComponent::~AnimationComponent()
{
	
}

void AnimationComponent::Init()
{
	// read file via ASSIMP
	
	
	m_Scene = m_Importer.ReadFile("Assets/Animations/shotgun/05_Combat_Shotgun@Run.fbx", aiProcess_Triangulate | 
		aiProcess_FlipUVs);
	// check for errors
	if (!m_Scene || !m_Scene->mRootNode) // if is Not Zero
	{
		Logger::GetInstance().Log("ERROR::ASSIMP:: " + std::string(m_Importer.GetErrorString()));
		return;
	}

	m_Animation = std::make_shared<Animation>(m_Scene->mAnimations[0],m_Scene);
	m_Animator = std::make_shared<Animator>();
	m_Animator->PlayAnimation(m_Animation);
}

void AnimationComponent::Destroy()
{
	
}

void AnimationComponent::Tick(float dt)
{
	m_Animator->Tick(dt);

	m_Transforms.clear();
	m_Transforms.resize(80);
	auto boneMap = Model::m_BoneIdMap;
	auto jointTransformMap = m_Animator->GetFinalTransform();
	for (auto pair : jointTransformMap)
		m_Transforms[pair.first] = pair.second;
}

