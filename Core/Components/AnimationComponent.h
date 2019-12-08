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
class Joint;

class AnimationComponent : public BaseComponent,public UserInputListener
{
	
public:

	AnimationComponent(const std::string& animationName);
	~AnimationComponent();
	void Init()override;
	void Destroy()override;
	void Tick(float dt)override;


	float GetScaleFactor(float lastTimeStamp, float nextTimeStamp);
	glm::mat4 InterpolateToNextAnimation(Joint* jointFrom,Joint* jointTo);
	glm::mat4 LerpPos(Joint* jointFrom,Joint* jointTo,float scaleFactor);
	glm::mat4 SlerpRot(Joint* jointFrom, Joint* jointTo,float scaleFactor);
	glm::mat4 LerpScale(Joint* jointFrom, Joint* jointTo,float scaleFactor);

	void CalculateJointTransform(const AssimpNodeData* node, glm::mat4 parentTransform);
	void PlayAnimation(std::shared_ptr<Animation> pAnimation);

	std::vector<glm::mat4> GetPoseTransforms() 
	{ 
		return m_Transforms;  
	}

	void OnKeyAction(int key,int action)override;

	ComponentID GetID()override { return s_ID; }
	static ComponentID s_ID;

	//TEMP function

	void PlayNextAnimation()
	{
		m_NextAnimation = m_Animations[++currentIndex % 3];
		isSwitchingAnimation = true;
		elapsedSwitchTime = 0;

	}

	
private:

	std::string m_AnimName;
	std::vector<glm::mat4> m_Transforms;
	std::shared_ptr<Animation> m_CurrentAnimation;
	std::shared_ptr<Animation> m_NextAnimation;
	std::vector<std::shared_ptr<Animation>> m_Animations;
	short currentIndex = 0; //<----remove it 
	float m_CurrentTime;
	float m_DeltaTime;

	float elapsedSwitchTime;
	float maxSwitchedDuration;
	bool isSwitchingAnimation;
	float switchSpeed;

	
};