#include "AnimationComponent.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include "../Utils/Logger.h"
#include <assimp/postprocess.h>

#include "../Animation/Animation.h"

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
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene;
	scene = importer.ReadFile(("Assets/Animations/" + m_AnimName).c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs);
	// check for errors
	if (!scene || !scene->mRootNode) // if is Not Zero
	{
		Logger::GetInstance()->Log("ERROR::ASSIMP:: " + std::string(importer.GetErrorString()));
		return;
	}

	m_Animation = new Animation(scene->mAnimations[0],scene);

}

void AnimationComponent::Destroy()
{
	delete m_Animation;
}

void AnimationComponent::Tick(float dt)
{
	m_Animation->Tick(dt);
	auto jointTransformMap = m_Animation->GetFinalTransform();
	for (auto pair : jointTransformMap)
		m_Transforms[pair.first] = pair.second;
}

