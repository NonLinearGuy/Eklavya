#pragma once

#include "BaseNode.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/VertexArrayObject.h"

class Scene;
class SphereVAO;

class SphereNode : public BaseNode
{
public:
	SphereNode(ActorID id, BaseRenderComponent* renderComponent, ERenderGroup renderPass);
	~SphereNode();
	bool Init()override;
	void Destroy()override;
	void Render(Scene* scene)override;
private:
	SphereVAO* m_SphereVAO;
	int m_IndicesSize;
	float m_Radius;
};

