#pragma once


#pragma once

#include "BaseNode.h"
#include <glad/glad.h>
#include "../UserInputListener.h"
class Scene;

class ModelNode : public BaseNode,public UserInputListener
{
public:
	ModelNode(ActorID pActorID, BaseRenderComponent* renderComponent, ERenderGroup renderPass);
	~ModelNode();
	bool Init()override;
	void Destroy()override;
	void Render(Scene* scene)override;
	void OnKeyAction(int key, int action)override;
private:
	class Model* m_Shotgun;
	bool mApplyNormaMap = false;
};