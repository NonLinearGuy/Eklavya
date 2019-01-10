#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>


enum ERenderGroup
{
	MIN = 0,
	SOLID = MIN,
	WATER,
	OUTLINED,
	BOUND_VOLUME,
	SKYBOX,
	MAX
};


class BaseRenderComponent;
class Scene;
class Material;
class IBoundVolume;


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
	
	inline std::shared_ptr<IBoundVolume> GetBoundVolume() { return m_BoundVolume; }
	

	virtual void AddChild(std::shared_ptr<BaseNode> pChild);
	virtual bool RemoveChild(std::shared_ptr<BaseNode> pChild);

	inline ERenderGroup GetRenderGroup()
	{
		return m_RenderGroup;
	}
	
	void SetTransform(const glm::mat4& toWorld);

	void SetParent(std::shared_ptr<BaseNode> parent) { m_Parent = parent; }
protected:
	
	glm::mat4 m_ToWorld;
	ERenderGroup m_RenderGroup;
	BaseRenderComponent* m_WeakRenderComponent;
	std::shared_ptr<BaseNode> m_Parent;
	std::vector<std::shared_ptr<BaseNode>> m_Children;
	ActorID m_ActorID;
	Material* m_Material;
	std::shared_ptr<IBoundVolume> m_BoundVolume;
};

