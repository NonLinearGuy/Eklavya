#pragma once

#include "BaseNode.h"
#include "../AssetManager/Cubemap.h"
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
private:
	VertexArrayObject m_VAO;
	int m_IndicesSize;
	std::shared_ptr<Cubemap> m_Cubemap;
};

