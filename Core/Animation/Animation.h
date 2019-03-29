#pragma once

#include <vector>
#include <map>
#include <assimp/scene.h>
#include "Joint.h"
#include <functional>
#include "../Model.h"


class Animation
{
public:
	Animation() = default;
	Animation(const aiAnimation* animation,const aiScene* scene);
	~Animation();
	void SetupJoints(const aiAnimation* animation);
	Joint* FindJoint(const std::string& name);
	aiNodeAnim* GetChannel();
	
	//getters
	inline glm::mat4 GetGlobalInverse() { return m_GlobalInverseTransform; }
	inline float GetTicksPerSecond() { return m_TicksPerSecond; }
	inline float GetDuration() { return m_Duration;}
	inline const aiNode* GetRootNode() { return m_RootNode; }
	inline std::map<std::string,int> GetBoneIDMap() { return Model::m_BoneIdMap; }
	inline std::map<std::string, glm::mat4> GetOffsetMap() { return Model::boneOffsetMap; }

private:
	
	float m_Duration;
	int m_TicksPerSecond;
	aiNode* m_RootNode;
	glm::mat4 m_GlobalInverseTransform;
	std::vector<Joint> m_Joints;
};
