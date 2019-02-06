#pragma once

#include "BaseNode.h"
#include <glad/glad.h>
class Scene;

class ArrowNode : public BaseNode
{
public:

	ArrowNode(ActorID actorID);
	~ArrowNode();
	bool Init()override;
	void Destroy()override;
	void PreRender(Scene* scene)override;
	void Render(Scene* scene)override;
	void PostRender(Scene* scene)override;
private:
	GLuint mVAO;
	GLuint mVBO;
};