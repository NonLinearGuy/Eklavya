#pragma once

#include <glm/glm.hpp>
#include <map>

using ModelID = unsigned;

struct BoneInfo
{
	/*
		For uniquely indentifying the bone and
		for indexing bone transformation in shaders
	*/
	int id;
	/*
		map from bone name to offset matrix.
		offset matrix transforms bone from bone space to local space
	*/
	glm::mat4 offset;

};

struct BonesData
{
	std::map< std::string, BoneInfo > m_BoneInfoMap;
	int m_BoneCount;
};

class ModelsBoneData
{
private:
	friend class Animation;
	friend class Model;
	static std::map<ModelID, BonesData> s_BonesDataMap;
};