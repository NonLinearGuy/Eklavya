#pragma once

#include "KeyFrame.h"
#include <vector>
#include <assimp/scene.h>
#include <list>

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
	void GetLastAndNextFrames(KeyFrame& one, KeyFrame& two,float animationTime );
	float CalculateProgression(const KeyFrame& one,const KeyFrame& two,float animationTime);
	glm::mat4 Interpolate(const KeyFrame& lastFrame,const KeyFrame& nextFrame,float scale);
	std::vector<KeyFrame> m_KeyFrames;
	glm::mat4 m_LocalTransform;
	std::string m_Name;
	std::vector<Joint*> m_Children;
	Joint* m_Parent;
	int m_ID;
};

