#include "Animation.h"
#include "Joint.h"
#include "../Model.h"
#include <glm/gtc/type_ptr.hpp>
#include "../Debugging/Diagnostics.h"

Animation::Animation(const aiAnimation * animation,const aiScene* scene)
	:
	m_CurrentAnimationTime(0.0f)
{
	m_Animation = animation;
	m_Scene = scene;
	m_Duration = animation->mDuration;
	m_TicksPerSecond = animation->mTicksPerSecond;
	aiMatrix4x4 globalTransformation = m_Scene->mRootNode->mTransformation;
	globalTransformation = globalTransformation.Inverse();
	m_GlobalInverseTransformation = toGlmMat(globalTransformation);
	SetupJoints();
}

Animation::~Animation()
{
}

void Animation::SetupJoints()
{
	int size = m_Animation->mNumChannels;

	for (int i = 0; i < size; i++)
	{
		auto channel = m_Animation->mChannels[i];
		m_Joints.push_back(new Joint(channel->mNodeName.data,Model::m_BoneIdMap[channel->mNodeName.data],channel));
	}
}

Joint* Animation::FindJoint(const std::string& name)
{
	auto iter = std::find_if(m_Joints.begin(), m_Joints.end(),
		[&](Joint* joint)
	{
		return joint->GetJointName() == name;
	}
	);
	if (iter == m_Joints.end()) return nullptr;
	else return *iter;
}

void Animation::ReadNodeHierarchy(float animationTime, aiNode* node, glm::mat4 parentTransform)
{
	std::string nodeName = node->mName.data;
	glm::mat4 nodeTransform = toGlmMat(node->mTransformation);
	Joint* joint = FindJoint(node->mName.data);
	if (joint)
	{
		joint->Update(animationTime);
		nodeTransform = joint->GetLocalTransform();
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransform;
	auto& boneIDMap = Model::m_BoneIdMap;
	if (boneIDMap.find(nodeName) != boneIDMap.end()) 
	{
		int index = boneIDMap[nodeName];
		auto boneMap = Model::boneOffsetMap;
		glm::mat4 offset = boneMap[nodeName];
		m_FinalTransforms[index] = m_GlobalInverseTransformation * globalTransformation * offset;
	}


	for (int i = 0; i < node->mNumChildren; i++)
	{
		ReadNodeHierarchy(animationTime, node->mChildren[i], globalTransformation);
	}

}

void Animation::Tick(float delta)
{
	m_FinalTransforms.clear();
	m_CurrentAnimationTime += delta;
	m_CurrentAnimationTime = fmod(m_CurrentAnimationTime,m_Duration);
	DiagManager::sGeneralDiagsMap[KEY_DELTA] = std::to_string(m_CurrentAnimationTime);
	ReadNodeHierarchy(m_CurrentAnimationTime, m_Scene->mRootNode, glm::mat4(1.0f));
}


/*THANKS TO THIS GUY FOR FIGURING OUT THE CONVERSION :
https://lechior.blogspot.com/2017/05/skeletal-animation-using-assimp-opengl.html*/

glm::mat4 Animation::toGlmMat(aiMatrix4x4 from)
{
	glm::mat4 to;
	//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
	to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
	to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
	to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
	to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
	return to;
}




