#include "AnimationComponent.h"

#include "../Utils/Logger.h"
#include <assimp/postprocess.h>
#include "../Model.h"
#include "../Animation/Animation.h"

ComponentID AnimationComponent::s_ID = 9;

AnimationComponent::AnimationComponent(const std::string & animationName)
	:
	m_AnimName(animationName)
{
	m_Transforms.reserve(110);

	for (int i = 0; i < 110; i++)
		m_Transforms.push_back(glm::mat4(1.0f));

	
}

AnimationComponent::~AnimationComponent()
{
	
}

void AnimationComponent::Init()
{
	// read file via ASSIMP
	
	
	m_Scene = m_Importer.ReadFile("Assets/Animations/dancing.dae", aiProcess_Triangulate | 
		aiProcess_FlipUVs);
	// check for errors
	if (!m_Scene || !m_Scene->mRootNode) // if is Not Zero
	{
		Logger::GetInstance()->Log("ERROR::ASSIMP:: " + std::string(m_Importer.GetErrorString()));
		return;
	}

	m_Animation = new Animation(m_Scene->mAnimations[0],m_Scene);
}

void AnimationComponent::Destroy()
{
	delete m_Animation;
}

void AnimationComponent::Tick(float dt)
{
	m_Animation->Tick(dt);

	m_Transforms.clear();
	m_Transforms.resize(110);
	auto boneMap = Model::m_BoneIdMap;
	auto jointTransformMap = m_Animation->GetFinalTransform();
	for (auto pair : jointTransformMap)
		m_Transforms[pair.first] = pair.second;
}

