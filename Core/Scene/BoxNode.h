#pragma once

#include "BaseNode.h"

#include "../Renderer/Texture2D.h"
#include "../Renderer/VertexArrayObject.h"

class BoxNode : public BaseNode
{
public:
	BoxNode(ActorID pActorID, BaseRenderComponent* renderComponent,ERenderGroup renderPass);
	~BoxNode();
	bool Init()override;
	void Destroy()override;
	void Tick(Scene* scene,float deltaTime)override;;
	void Render(Scene* scene)override;
private:
	VertexArrayObject m_VAOConfig;
	std::shared_ptr<Texture2D> m_FloorTexture;
};