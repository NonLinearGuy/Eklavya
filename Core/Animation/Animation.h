#pragma once

#include <vector>
#include <map>
#include <assimp/scene.h>
#include "Joint.h"
#include <functional>

struct MyComparator
{
	bool operator()(int first, int second) const
	{
		return first < second;
	}
};

class Animation
{
public:
	Animation(const aiAnimation* animation,const aiScene* scene);
	~Animation();
	void Tick(float delta);
	glm::mat4 toGlmMat(aiMatrix4x4 mat);
	std::map<int, glm::mat4,MyComparator> GetFinalTransform() { return m_FinalTransforms; }
	void PrintHierarchy();

private:
	
	std::map<int, glm::mat4,MyComparator> m_FinalTransforms;
	float m_Duration;
	int m_TicksPerSecond;
	float m_CurrentAnimationTime;
	Joint* m_RootJoint;
	std::vector<Joint*> m_JointList;
	glm::mat4 m_InitialTransform;
private:
	void SetupHierarchy(const aiAnimation* animation,const aiScene* scene);
	const aiNode* GetJointParentNodeFromScene(const std::string& jointName,const aiScene* scene,bool& isRootNode);
	bool IsPresentInList(const std::string& jointName);
	Joint* GetJointFromList(const std::string& jointName);
	bool IsJointInvolved(const aiAnimation* animation,const std::string& jointName);
	void UpdateFinalTransformRescursively(Joint* rootJoint,glm::mat4 parentTransform);
};
