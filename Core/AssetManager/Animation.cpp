#include "Animation.h"
#include "../Animation/Joint.h"
#include "../AssetManager/Model.h"
#include <glm/gtc/type_ptr.hpp>
#include "../Debugging/Diagnostics.h"
#include "../Helpers.h"

Animation::Animation(const aiAnimation * animation,const aiScene* scene)
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

	for (int i = 0; i < size; i++)
	{
		auto channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;
		if (Model::m_BoneInfoMap.find(boneName) == Model::m_BoneInfoMap.end())
		{
			Model::m_BoneInfoMap[boneName].id = Model::m_BoneCount;
			Model::m_BoneCount++;
		}
		m_Joints.push_back(Joint(channel->mNodeName.data,Model::m_BoneInfoMap[channel->mNodeName.data].id,channel));
	}
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

