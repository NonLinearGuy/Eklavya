#include "AnimationComponent.h"

#include "../Utils/Logger.h"
#include <assimp/postprocess.h>
#include "../AssetManager/Model.h"
#include "../AssetManager/Animation.h"
#include "../Animation/Animator.h"
#include <GLFW/glfw3.h>

ComponentID AnimationComponent::s_ID = 9;

AnimationComponent::AnimationComponent(const std::string & animationName)
	
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
	// read file via ASSIMP
	
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile("Assets/Animations/Berserker/ch_h_Lancer_idle.fbx", aiProcess_Triangulate | 
		aiProcess_FlipUVs);
	assert(scene && scene->mRootNode);

	m_Animation = std::make_shared<Animation>(scene->mAnimations[0], scene);
	m_Animations.push_back(std::make_shared<Animation>(scene->mAnimations[0], scene));

	m_Animator = std::make_shared<Animator>();
	m_Animator->PlayAnimation(m_Animation);
	
	scene = importer.ReadFile("Assets/Animations/Berserker/ch_h_Lancer_Run.fbx", aiProcess_Triangulate |
		aiProcess_FlipUVs);
	assert(scene && scene->mRootNode);

	m_Animations.push_back(std::make_shared<Animation>(scene->mAnimations[0], scene));

	scene = importer.ReadFile("Assets/Animations/Berserker/ch_h_Lancer_Stun.fbx", aiProcess_Triangulate |
		aiProcess_FlipUVs);
	assert(scene && scene->mRootNode);

	m_Animations.push_back(std::make_shared<Animation>(scene->mAnimations[0], scene));

	m_Animator = std::make_shared<Animator>();
	PlayIdle();
}

void AnimationComponent::Destroy()
{
	
}

void AnimationComponent::OnKeyAction(int key, int action)
{
	
}

void AnimationComponent::PlayIdle()
{
	m_Animator->PlayAnimation(m_Animations[0]);
}

void AnimationComponent::PlayRun()
{
	m_Animator->PlayAnimation(m_Animations[1]);
}

void AnimationComponent::PlayAttack()
{
	m_Animator->PlayAnimation(m_Animations[2]);
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

