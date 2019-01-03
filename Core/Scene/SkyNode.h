#pragma once

#include "BaseNode.h"
#include "../Renderer/Cubemap.h"
#include "../Renderer/VertexArrayObject.h"

class Scene;

class SkyNode : public BaseNode
{
public:
	SkyNode(ActorID id, BaseRenderComponent* renderComponent, ERenderGroup renderPass);
	~SkyNode();
	bool Init()override;
	void Destroy()override;
	void PreRender(Scene* scene)override;
	void Render(Scene* scene)override;
	void PostRender(Scene* scene)override;
private:
	VertexArrayObject m_VAO;
	int m_IndicesSize;
	Cubemap m_Cubemap;
};

