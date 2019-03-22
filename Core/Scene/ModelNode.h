#pragma once


#pragma once

#include "BaseNode.h"
#include <glad/glad.h>

class Scene;

class ModelNode : public BaseNode
{
public:
	ModelNode(ActorID pActorID, BaseRenderComponent* renderComponent, ERenderGroup renderPass);
	~ModelNode();
	bool Init()override;
	void Destroy()override;
	void Render(Scene* scene)override;
private:
	GLuint mVAO;
	GLuint mVBO;
	class Model* m_Hands;
	class Model* m_Shotgun;
};