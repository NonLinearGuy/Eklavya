#pragma once

#pragma once

#include "BaseNode.h"

#include "../Renderer/Texture2D.h"
#include "../Renderer/VertexArrayObject.h"

class CuboidVAO;

class BoxBoundNode : public BaseNode
{
public:
	BoxBoundNode(ActorID pActorID, const glm::vec3& extents);
	~BoxBoundNode();
	void SetColor(const glm::vec3& color);
	bool Init()override;
	void Destroy()override;
	void Tick(Scene* scene, float deltaTime)override;;
	void PreRender(Scene* scene)override;
	void Render(Scene* scene)override;
	void PostRender(Scene* scene)override;
private:

	CuboidVAO* m_CubeVAO;
	glm::vec3 m_Color;
	std::weak_ptr<class Transform> m_Transform;
	glm::mat4 m_Scale;
};
