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
	void SetupJoints();
	Joint* FindJoint(const std::string& name);
	void ReadNodeHierarchy(float animationTime,aiNode* node,glm::mat4 parentTransform);
	aiNodeAnim* GetChannel();
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
	const aiScene* m_Scene;
	glm::mat4 m_GlobalInverseTransformation;
	const aiAnimation* m_Animation;
	std::vector<Joint*> m_Joints;
private:
	
};
