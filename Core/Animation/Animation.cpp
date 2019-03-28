#include "Animation.h"
#include "Joint.h"
#include "../Model.h"
#include <glm/gtc/type_ptr.hpp>
#include "../Debugging/Diagnostics.h"
#include "../Helpers.h"

Animation::Animation(const aiAnimation * animation,const aiScene* scene)
{
	m_Duration = animation->mDuration;
	m_RootNode = scene->mRootNode;
	m_TicksPerSecond = animation->mTicksPerSecond;
	aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
	globalTransformation = globalTransformation.Inverse();
	m_GlobalInverseTransform = GetGLMMat(globalTransformation);
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
		if (Model::m_BoneIdMap.find(boneName) == Model::m_BoneIdMap.end())
		{
			Model::m_BoneIdMap[boneName] = Model::m_BoneCount;
			Model::m_BoneCount++;
		}
		m_Joints.push_back(Joint(channel->mNodeName.data,Model::m_BoneIdMap[channel->mNodeName.data],channel));
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

