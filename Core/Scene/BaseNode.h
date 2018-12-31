#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>

class Scene;
class BaseRenderComponent;
class BaseNode;

enum ERenderPass
{
	MIN = 0,
	STATIC = MIN,
	DYNAMIC,
	SKYBOX,
	MAX
};

class BaseNode
{
public:
	using ActorID = unsigned int;

	BaseNode(ActorID actorID, BaseRenderComponent* renderComponent, ERenderPass renderPasss);
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

	inline ERenderPass GetRenderPass()
	{
		return m_RenderPass;
	}

	void SetParent(std::shared_ptr<BaseNode> parent) { m_Parent = parent; }
protected:
	ERenderPass m_RenderPass;
	BaseRenderComponent* m_WeakRenderComponent;
	std::shared_ptr<BaseNode> m_Parent;
	std::vector<std::shared_ptr<BaseNode>> m_Children;
	ActorID m_ActorID;
};

