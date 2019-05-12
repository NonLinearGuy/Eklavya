#pragma once

#include <vector>
#include <map>
#include <assimp/scene.h>
#include "../Animation/Joint.h"
#include <functional>
#include "../AssetManager/Model.h"
#include "AnimationData.h"
#include "IAsset.h"


struct AssimpNodeData
{
	glm::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpNodeData> children;
};

class Animation : public IAsset
{
public:
	Animation() = default;
	Animation(const std::string& assetName,
		int modelID,const aiAnimation* animation, const aiScene* scene);
	
	~Animation();
	Joint* FindJoint(const std::string& name);
	
	//getters
	inline glm::mat4 GetGlobalInverse() { return m_GlobalInverseTransform; }
	inline float GetTicksPerSecond() { return m_TicksPerSecond; }
	inline float GetDuration() { return m_Duration;}
	inline const AssimpNodeData& GetRootNode() { return m_RootNode; }
	inline const std::map<std::string,BoneInfo>& GetBoneIDMap() 
	{ 
		return m_BoneInfoMap;
	}

private:
	void SetupJoints(const aiAnimation* animation);
	void ReadHeirarchyData(AssimpNodeData& dest,const aiNode* src);
	float m_Duration;
	int m_TicksPerSecond;
	glm::mat4 m_GlobalInverseTransform;
	std::vector<Joint> m_Joints;
	AssimpNodeData m_RootNode;
	std::map<std::string, BoneInfo> m_BoneInfoMap;
	int m_ModelID;
};
