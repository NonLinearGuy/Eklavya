#pragma once

#include "BaseNode.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/VertexArrayObject.h"

class Scene;

class SphereNode : public BaseNode
{
public:
	SphereNode(ActorID id, BaseRenderComponent* renderComponent, ERenderPass renderPass);
	~SphereNode();
	bool Init()override;
	void Destroy()override;
	void PreRender(Scene* scene)override;
	void Render(Scene* scene)override;
	void PostRender(Scene* scene)override;
private:
	HipHop::ShaderProgram m_ShaderProgram;
	VertexArrayObject m_VAO;
	int m_IndicesSize;
	HipHop::Texture2D m_FloorTexture;
};
