#include "Animation.h"
#include "Joint.h"
#include "../Model.h"


Animation::Animation(const aiAnimation * animation,const aiScene* scene)
	:
	m_CurrentAnimationTime(0.0f)
{
	
	m_Duration = animation->mDuration;
	m_TicksPerSecond = animation->mTicksPerSecond;
	SetupHierarchy(animation,scene);
}

const aiNode * Animation::GetJointParentNodeFromScene(const std::string & boneName, const aiScene * scene,bool& isRootNode)
{
	const aiNode* jointNode = scene->mRootNode->FindNode(boneName.c_str());
	const aiNode* parent = nullptr;
	parent = jointNode->mParent;
	while (true)
	{
		if (parent == scene->mRootNode)
		{
			isRootNode = true;
			parent = nullptr;
			break;
		}
		if (parent->mName.length > 0)
			break;
		parent = parent->mParent;
	}

	return parent;
}

bool Animation::IsPresentInList(const std::string & jointName)
{
	auto iter = std::find_if(m_JointList.begin(), m_JointList.end(), 
		[&](Joint* joint) 
	{
		return joint->GetJointName() == jointName; 
	}
	);

	return iter != m_JointList.end();
}

void Animation::PrintHierarchy()
{
	m_RootJoint->PrintHeirarchy();
}

Joint* Animation::GetJointFromList(const std::string& jointName)
{
	auto iter = std::find_if(m_JointList.begin(), m_JointList.end(),
		[&](Joint* joint)
	{
		return joint->GetJointName() == jointName;
	}
	);

	return *iter;
}

bool Animation::IsJointInvolved(const aiAnimation* animation,const std::string& jointName)
{
	for (int channelIndex = 0; channelIndex < animation->mNumChannels; ++channelIndex)
	{
		if (animation->mChannels[channelIndex]->mNodeName == aiString(jointName))
			return true;
	}
	return false;
}


void Animation::SetupHierarchy(const aiAnimation* animation,const aiScene* scene)
{
	   /*
		Step#1 : Check if the joint is already exist, If yes then skip step 2 and 3
		Step#2 : Create joint out of boneName
		Step#3 : Add Joint in the list
		Step#4 : Look for corresponding node in scene
		Step#5 : If node is found, then traverse to Its parent bone node
				- Get name and check if this exist in list
				- If yes then skip to last step
				- Create a joint node Push it to the list
				- Add channel node as a child to parent joint
				
		Last Step : Check every joint in the list and whatever occurs first in scene heirarchy assign it as RootNode
		*/
	for (int channelIndex = 0; channelIndex < animation->mNumChannels; ++channelIndex)
	{
		const aiNodeAnim* channel = animation->mChannels[channelIndex];
		std::string jointName = channel->mNodeName.C_Str();
		bool present = IsPresentInList(jointName);
		Joint* childJoint = nullptr;
		if (!present)
		{
			auto map = Model::m_BoneIdMap;
			int id = Model::m_BoneIdMap[jointName];
			childJoint = new Joint(jointName,id,channel);
			m_JointList.push_back(childJoint);
		}
		else
		{
			childJoint = GetJointFromList(jointName);
		}
		
		if (childJoint->GetParent() != nullptr) continue;
		
		bool isRootNode = false;
		const aiNode* parentNode = GetJointParentNodeFromScene(jointName,scene,isRootNode);

		//have we found the root node for this animation joint hierarchy?
		if (parentNode == nullptr)
		{
			m_InitialTransform = glm::mat4(1.0f);
			m_RootJoint = childJoint;
			continue;
		}
		std::string parentName = parentNode->mName.C_Str();
		bool IsInvolved = IsJointInvolved(animation,parentName);
		if (!IsInvolved)
		{
			m_InitialTransform = toGlmMat(parentNode->mTransformation);
			m_RootJoint = childJoint;
			continue;
		}

		present = IsPresentInList(parentName);

		Joint* parentJoint = nullptr;
		if (!present)
		{
			int id = Model::m_BoneIdMap[parentName];
			parentJoint = new Joint(parentName, id, channel);
			m_JointList.push_back(parentJoint);
		}
		else
		{
			parentJoint = GetJointFromList(parentName);
		}

		parentJoint->AddChild(childJoint);
		childJoint->SetParent(parentJoint);
	}

	PrintHierarchy();

	m_InitialTransform = toGlmMat(scene->mRootNode->mTransformation);
}


Animation::~Animation()
{
}

void Animation::UpdateFinalTransformRescursively(Joint * rootJoint, glm::mat4 parentTransform)
{
	glm::mat4 modelSpaceTransform = parentTransform * rootJoint->GetLocalTransform();
	int id = rootJoint->GetJointID();
	m_FinalTransforms[id] = glm::inverse(modelSpaceTransform);
	auto children = rootJoint->GetChildren();
	for (auto child : children)
	{
		UpdateFinalTransformRescursively(child, modelSpaceTransform);
	}
}

void Animation::Tick(float delta)
{
	m_FinalTransforms.clear();
	m_CurrentAnimationTime += delta;
	m_CurrentAnimationTime = glm::modf(m_CurrentAnimationTime,m_Duration);
	m_RootJoint->Update(m_CurrentAnimationTime);
	UpdateFinalTransformRescursively(m_RootJoint,m_InitialTransform);
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
	to = glm::transpose(to);
	return to;
}
