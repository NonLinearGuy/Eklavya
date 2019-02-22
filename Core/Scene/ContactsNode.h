#pragma once

#include "BaseNode.h"
#include <glad/glad.h>
class Scene;
class IEventData;

class ContactsNode : public BaseNode
{
public:
	ContactsNode(ActorID actorID);
	~ContactsNode();
	bool Init()override;
	void Destroy()override;
	void PreRender(Scene* scene)override;
	void Render(Scene* scene)override;
	void PostRender(Scene* scene)override;

	void OnContactsUpdated(std::shared_ptr<IEventData> eventData);

private:
	bool m_ContactsUpdated;
	int m_InstanceSize;
	GLuint mPointVAO;
	GLuint mPointVBO;
	GLuint mNormalVAO;
	GLuint mNormalVBO;
};