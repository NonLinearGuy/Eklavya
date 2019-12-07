#pragma once

#include "BaseComponent.h"
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include "../UserInputListener.h"

class Animation;
class AssimpNodeData;

class AnimationComponent : public BaseComponent,public UserInputListener
{
	
public:
	AnimationComponent(const std::string& animationName);
	~AnimationComponent();
	void Init()override;
	void Destroy()override;
	void Tick(float dt)override;


	void CalculateJointTransform(const AssimpNodeData* node, glm::mat4 parentTransform);
	void PlayAnimation(std::shared_ptr<Animation> pAnimation);

	std::vector<glm::mat4> GetPoseTransforms() 
	{ 
		return m_Transforms;  
	}

	void OnKeyAction(int key,int action)override;

	ComponentID GetID()override { return s_ID; }
	static ComponentID s_ID;

	
private:

	std::string m_AnimName;
	std::vector<glm::mat4> m_Transforms;
	std::shared_ptr<Animation> m_CurrentAnimation;
	float m_CurrentTime;
	
};