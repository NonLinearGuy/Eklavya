#pragma once

#include "BaseComponent.h"

class BaseNode;

class BaseRenderComponent : public BaseComponent
{
public:
	BaseRenderComponent();
	~BaseRenderComponent();
	virtual std::shared_ptr<BaseNode> CreateBaseNode() = 0;

	ComponentID GetID()override { return s_ID; }
	static ComponentID s_ID;
protected:
	std::shared_ptr<BaseNode> m_BaseNode;
};


class BoxRenderComponent : public BaseRenderComponent
{
public:
	BoxRenderComponent();
	~BoxRenderComponent();
	static ComponentID s_ID;
	std::shared_ptr<BaseNode> CreateBaseNode() override;
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
	std::shared_ptr<BaseNode> CreateBaseNode() override;
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
	std::shared_ptr<BaseNode> CreateBaseNode() override;
};

