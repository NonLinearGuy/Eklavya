#pragma once

#include "KeyFrame.h"
#include <vector>
#include <assimp/scene.h>
#include <list>


struct KeyPosition
{
	glm::vec3 position;
	float timeStamp;
};

struct KeyRotation
{
	glm::quat orientation;
	float timeStamp;
};

struct KeyScale
{
	glm::vec3 scale;
	float timeStamp;
};

class Joint
{
public:
	Joint() {}
	Joint(const std::string& name,int ID,const aiNodeAnim* channel);
	~Joint();
	void Update(float animationTime);
	glm::mat4 GetLocalTransform() { return m_LocalTransform; }
	std::string GetJointName() { return m_Name; }
	int GetJointID() { return m_ID; }
	std::vector<Joint*> GetChildren() { return m_Children; }
	void AddChild(Joint* child);
	inline void SetParent(Joint* parent) { m_Parent = parent; }
	inline const Joint* GetParent() { return m_Parent; }
	void PrintHeirarchy();
private:
	int GetPositionIndex(float animationTime);
	int GetRotationIndex(float animationTime);
	int GetScaleIndex(float animationTime);
	float GetScaleFactor(float lastTimeStamp,float nextTimeStamp,float animationTime);
	glm::mat4 InterpolatePosition(float animationTime);
	glm::mat4 InterpolateRotation(float animationTime);
	glm::mat4 InterpolateScaling(float animationTime);

	std::vector<KeyFrame> m_KeyFrames;
	std::vector<KeyPosition> m_Positions;
	std::vector<KeyRotation> m_Rotations;
	std::vector<KeyScale> m_Scales;
	int m_NumPositions;
	int m_NumRotations;
	int m_NumScalings;

	glm::mat4 m_LocalTransform;
	std::string m_Name;
	std::vector<Joint*> m_Children;
	Joint* m_Parent;
	int m_ID;
};

