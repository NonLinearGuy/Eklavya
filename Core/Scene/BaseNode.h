#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>


enum ERenderGroup
{
	MIN = 0,
	SOLID = MIN,
	OUTLINED,
	WATER,
	SKYBOX,
	MAX
};


class BaseRenderComponent;
class Scene;
class Material;

class BaseNode
{
public:
	using ActorID = unsigned int;

	BaseNode(ActorID actorID, BaseRenderComponent* renderComponent, ERenderGroup renderPasss);
	~BaseNode();

	virtual bool Init();
	virtual void Destroy();
	virtual void Tick(Scene* scene,float deltaTime);
	virtual void PreRender(Scene* scene);
	virtual void Render(Scene* scene);
	virtual void RenderChildren(Scene* scene);
	virtual void PostRender(Scene* scene);
	
	inline bool IsInsideFrustum(Scene* scene) { return true; }

	virtual void AddChild(std::shared_ptr<BaseNode> pChild);
	virtual bool RemoveChild(std::shared_ptr<BaseNode> pChild);

	inline ERenderGroup GetRenderGroup()
	{
		return m_RenderGroup;
	}

	void SetParent(std::shared_ptr<BaseNode> parent) { m_Parent = parent; }
protected:
	ERenderGroup m_RenderGroup;
	BaseRenderComponent* m_WeakRenderComponent;
	std::shared_ptr<BaseNode> m_Parent;
	std::vector<std::shared_ptr<BaseNode>> m_Children;
	ActorID m_ActorID;
	Material* m_Material;
};

