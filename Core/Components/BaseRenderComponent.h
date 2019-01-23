#pragma once

#include "BaseComponent.h"
#include <glm/glm.hpp>

class BaseNode;


class BaseRenderComponent : public BaseComponent
{
public:
	BaseRenderComponent();
	~BaseRenderComponent();
    inline glm::vec3 GetColor() { return m_Color; }
	inline void SetColor(const glm::vec3& color) { m_Color = color; }
	inline void SetTexturePath(const std::string& path) { m_TexturePath = path; }
	inline std::string GetTexturePath() { return m_TexturePath; }
	virtual void CreateBaseNode() = 0;
	inline std::shared_ptr<BaseNode> GetBaseNode() { return m_BaseNode; }

	ComponentID GetID()override { return s_ID; }
	static ComponentID s_ID;
protected:
	std::shared_ptr<BaseNode> m_BaseNode;
	glm::vec3 m_Color;
	std::string m_TexturePath;
};


class BoxRenderComponent : public BaseRenderComponent
{
public:
	BoxRenderComponent();
	~BoxRenderComponent();
	static ComponentID s_ID;
	void CreateBaseNode() override;
private:

};

class SphereRenderComponent : public BaseRenderComponent
{
public:
	SphereRenderComponent(float pRadius = 1.0f,int pStacks = 10,int pSectors = 10);
	~SphereRenderComponent();
	static ComponentID s_ID;
	inline unsigned int GetStacks() { return m_Stacks; }
	inline unsigned int GetSectors() { return m_Sectors; }
	inline float GetRadius() { return m_Radius; }
	void CreateBaseNode() override;
private:
	int m_Stacks;
	int m_Sectors;
	float m_Radius;
};

class SkyRenderComponent : public BaseRenderComponent
{
public:
	SkyRenderComponent();
	~SkyRenderComponent();
	static ComponentID s_ID;
	void CreateBaseNode() override;
};


class BoxColliderRenderComponent : public BaseRenderComponent
{
public:
	BoxColliderRenderComponent();
	~BoxColliderRenderComponent();
	static ComponentID s_ID;
	void CreateBaseNode() override;
private:

};

class SphereColliderRenderComponent : public BaseRenderComponent
{
public:
	SphereColliderRenderComponent(float pRadius = 1.0f, int pStacks = 10, int pSectors = 10);
	~SphereColliderRenderComponent();
	static ComponentID s_ID;
	inline unsigned int GetStacks() { return m_Stacks; }
	inline unsigned int GetSectors() { return m_Sectors; }
	inline float GetRadius() { return m_Radius; }
	void CreateBaseNode() override;
private:
	int m_Stacks;
	int m_Sectors;
	float m_Radius;
};
