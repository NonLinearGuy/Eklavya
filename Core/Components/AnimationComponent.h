#pragma once

#include "BaseComponent.h"
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

class Animation;


class AnimationComponent : public BaseComponent
{
public:
	AnimationComponent(const std::string& animationName);
	~AnimationComponent();
	void Init()override;
	void Destroy()override;
	void Tick(float dt)override;
	std::vector<glm::mat4> GetPoseTransforms() { return m_Transforms;  }

	

	ComponentID GetID()override { return s_ID; }
	static ComponentID s_ID;

private:

	std::vector<glm::mat4> m_Transforms;
	Animation* m_Animation;
	std::string m_AnimName;
	Assimp::Importer m_Importer;
	const aiScene* m_Scene;
};