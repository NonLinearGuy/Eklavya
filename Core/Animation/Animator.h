#pragma once

#include <vector>
#include <map>
#include <assimp/scene.h>
#include <functional>
#include <glm/glm.hpp>
#include "../AssetManager/Animation.h"
#include <memory>


class Animator
{
public:
	Animator();
	~Animator();

	void CalculateJointTransform(const AssimpNodeData& node, glm::mat4 parentTransform);
	void PlayAnimation(std::shared_ptr<Animation> pAnimation);
	void Tick(float delta);

	std::map<int, glm::mat4> GetFinalTransform() 
	{ 
		return m_FinalTransforms; 
	}

private:
	std::map<int, glm::mat4> m_FinalTransforms;
	std::shared_ptr<Animation> m_CurrentAnimation;
	float m_CurrentTime;
};
