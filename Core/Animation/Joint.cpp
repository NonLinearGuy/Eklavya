#include "Joint.h"
#include "Animation.h"
#include "../Utils/Logger.h"
#include <iostream>
#include "../Model.h"

Joint::Joint(const std::string& name, int ID, const aiNodeAnim* channel)
	:
	m_Name(name),
	m_ID(ID),
	m_Parent(nullptr),
	m_LocalTransform(1.0f)
{
	for (int index = 0; index < channel->mNumPositionKeys; ++index)
	{
		aiQuaternion orientation = channel->mRotationKeys[index].mValue;
		aiVector3D position = channel->mPositionKeys[index].mValue;
		aiVector3D scale = channel->mScalingKeys[index].mValue;
		float timeStamp = channel->mPositionKeys[index].mTime;
		KeyFrame newKeyFrame(orientation,position,scale,timeStamp);
		m_KeyFrames.push_back(newKeyFrame);
	}
}

Joint::~Joint()
{
}

void SetBoneDat(aiNode* node)
{
	
}


void Joint::AddChild(Joint* child)
{
	m_Children.push_back(child);
}

void Joint::GetLastAndNextFrames(KeyFrame& last, KeyFrame& next,float animationTime)
{
	int size = m_KeyFrames.size();
	for (int index = 0 ; index < size ; ++index)
	{
		if (animationTime >= m_KeyFrames[index].m_TimeStamp && animationTime < m_KeyFrames[index + 1].m_TimeStamp)
		{
			last = m_KeyFrames[index];
			next = m_KeyFrames[index + 1];
			break;
		}
	}
}

float Joint::CalculateProgression(const KeyFrame& last, const KeyFrame& next,float animationTime)
{
	float scale = 0.0f;
	float midWayLength = animationTime - last.m_TimeStamp;
	float framesDiff = next.m_TimeStamp - last.m_TimeStamp;
	scale = midWayLength / framesDiff;
	return scale;
}

glm::mat4 Joint::Interpolate(const KeyFrame& last, const KeyFrame& next, float scale)
{
	glm::quat finalOrientation =  glm::slerp(last.m_Orientation,next.m_Orientation,scale);
	glm::vec3 finalPosition = glm::mix(last.m_Position,next.m_Position,scale);
	glm::vec3 finalScale = glm::mix(last.m_Scale,next.m_Scale,scale);

	finalOrientation = glm::normalize(finalOrientation);

	glm::mat4 rotation = glm::toMat4(finalOrientation);
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f),finalScale);
	glm::mat4 translation = glm::translate(glm::mat4(1.0f),finalPosition);

	return translation*rotation * scaleMat;
}


void Joint::Update(float animationTime)
{
	KeyFrame lastFrame;
	KeyFrame nextFrame;
	GetLastAndNextFrames(lastFrame,nextFrame,animationTime);
	float scale = CalculateProgression(lastFrame,nextFrame,animationTime);
	m_LocalTransform = Interpolate(lastFrame, nextFrame, scale);
	
}

void Joint::PrintHeirarchy()
{
	static int tabs = 1;
	//Logger::GetInstance()->Log("\t%s\n",m_Name.c_str());
	
	for (int i = 0; i < tabs; i++)
		std::cout << " -";
	std::cout << m_Name << std::endl;
	tabs++;
	for (auto child : m_Children)
		child->PrintHeirarchy();
	tabs--;
}
