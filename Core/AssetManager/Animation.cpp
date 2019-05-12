#include "Animation.h"
#include "../Animation/Joint.h"
#include "../AssetManager/Model.h"
#include <glm/gtc/type_ptr.hpp>
#include "../Debugging/Diagnostics.h"
#include "../Helpers.h"
#include "AnimationData.h"

Animation::Animation(const std::string& assetName,
	int modelID,const aiAnimation * animation,const aiScene* scene)
	:
	IAsset(EAssetType::ANIMATION,assetName),
	m_ModelID(modelID)
{
	m_Duration = animation->mDuration;
	m_TicksPerSecond = animation->mTicksPerSecond;
	aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
	globalTransformation = globalTransformation.Inverse();
	m_GlobalInverseTransform = GetGLMMat(globalTransformation);
	ReadHeirarchyData(m_RootNode,scene->mRootNode);
	SetupJoints(animation);
}

Animation::~Animation()
{
}

void Animation::SetupJoints(const aiAnimation* animation)
{
	int size = animation->mNumChannels;

	auto& modelDataMap = ModelsBoneData::s_BonesDataMap[m_ModelID];
	auto& boneInfoMap = modelDataMap.m_BoneInfoMap;
	int& boneCount = modelDataMap.m_BoneCount;

	for (int i = 0; i < size; i++)
	{
		auto channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;
	
		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			boneInfoMap[boneName].id = boneCount;
			boneCount++;
		}
		m_Joints.push_back(Joint(channel->mNodeName.data,
			boneInfoMap[channel->mNodeName.data].id,channel));
	}

	m_BoneInfoMap = boneInfoMap;
}

void Animation::ReadHeirarchyData(AssimpNodeData& dest,const aiNode* src)
{
	assert( src );

	dest.name = src->mName.data;
	dest.transformation = GetGLMMat(src->mTransformation);
	dest.childrenCount = src->mNumChildren;

	for (int i = 0; i < src->mNumChildren; i++)
	{
		AssimpNodeData newData;
		ReadHeirarchyData(newData,src->mChildren[i]);
		dest.children.push_back(newData);
	}
}


Joint* Animation::FindJoint(const std::string& name) 
{
	auto iter = std::find_if(m_Joints.begin(), m_Joints.end(),
		[&](const Joint& joint)
	{
		return joint.GetJointName() == name;
	}
	);
	if (iter == m_Joints.end()) return nullptr;
	else return &(*iter);
}

