#pragma once

#include "BaseNode.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Cubemap.h"
#include "../Renderer/VertexArrayObject.h"

class Scene;

class SkyNode : public BaseNode
{
public:
	SkyNode(ActorID id, BaseRenderComponent* renderComponent, ERenderPass renderPass);
	~SkyNode();
	bool Init()override;
	void Destroy()override;
	void PreRender(Scene* scene)override;
	void Render(Scene* scene)override;
	void PostRender(Scene* scene)override;
private:
	HipHop::ShaderProgram m_ShaderProgram;
	VertexArrayObject m_VAO;
	int m_IndicesSize;
	Cubemap m_Cubemap;
};

