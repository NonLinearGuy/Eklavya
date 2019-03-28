#pragma once

#include "BaseNode.h"

#include "../AssetManager/Texture2D.h"
#include "../Renderer/VertexArrayObject.h"

class CuboidVAO;

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
	
	CuboidVAO* m_CubeVAO;
};