#pragma once

#include "BaseNode.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/VertexArrayObject.h"

class Scene;

class SphereNode : public BaseNode
{
public:
	SphereNode(ActorID id, BaseRenderComponent* renderComponent, ERenderGroup renderPass);
	~SphereNode();
	bool Init()override;
	void Destroy()override;
	void Render(Scene* scene)override;
private:
	VertexArrayObject m_VAO;
	int m_IndicesSize;
	std::shared_ptr<Texture2D> m_FloorTexture;
	float m_Radius;
};
