#include "ContactsNode.h"
#include "../Renderer/VertexArrayObject.h"
#include "Scene.h"
#include "../Components/GameActor.h"
#include "../Components/Transform.h"
#include "../Helpers.h"
#include "../Engine.h"
#include "../Renderer/GLRenderer.h"
#include "../Event/EventDispatcher.h"
#include "../Physics/CollisionDetector.h"


ContactsNode::ContactsNode(ActorID actorID) :BaseNode(actorID, nullptr, ERenderGroup::CONTACTS)
{

}

ContactsNode::~ContactsNode()
{

}

bool ContactsNode::Init()
{

	glGenVertexArrays(1, &mPointVAO);
	glBindVertexArray(mPointVAO);

	glGenBuffers(1, &mPointVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mPointVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 50, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glGenVertexArrays(1, &mNormalVAO);
	glBindVertexArray(mNormalVAO);

	glGenBuffers(1, &mNormalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mNormalVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 100, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	RegisterToEvent<ContactsNode>(this,&ContactsNode::OnContactsUpdated,EEventType::CONTACTS_UPDATED);

	m_ContactsUpdated = false;

	return true;
}

void ContactsNode::Destroy()
{
	UnRegisterToEvent<ContactsNode>(this, &ContactsNode::OnContactsUpdated, EEventType::CONTACTS_UPDATED);

	glDeleteBuffers(1, &mPointVBO);
	glDeleteVertexArrays(1, &mPointVAO);
}

void ContactsNode::PreRender(Scene* scene)
{
	
}

void ContactsNode::OnContactsUpdated(std::shared_ptr<IEventData> eventData)
{
	auto data = std::static_pointer_cast<EventContactsUpdated>(eventData);
	assert(data);
	auto contactsVec = data->m_ContactsWorld;
	if (contactsVec.empty()) return;
	
	std::vector<glm::vec3> lineVertices;
	std::vector<glm::vec3> points;

	for (auto cData : contactsVec)
	{
		auto startPoint = cData.m_Point;
		auto normal = cData.m_Normal;
		auto endPoint = startPoint + (cData.m_Normal * 20.0f);
		lineVertices.push_back(startPoint);
		lineVertices.push_back(endPoint);
		points.push_back(startPoint);
	}

	glBindBuffer(GL_ARRAY_BUFFER,mPointVBO);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(glm::vec3) * points.size(),&points[0]);
	glBindBuffer(GL_ARRAY_BUFFER, mNormalVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * lineVertices.size(), &lineVertices[0]);

	m_ContactsUpdated = true;
	m_InstanceSize = contactsVec.size();
}


void ContactsNode::Render(Scene * scene)
{
	if (m_ContactsUpdated)
	{

		scene->GetRenderer()->GetActiveProgram()->SetVec3("color",glm::vec3(1.0f,0.0f,0.0f));
	glBindVertexArray(mPointVAO);
	glDrawArrays(GL_POINTS, 0, m_InstanceSize);

	scene->GetRenderer()->GetActiveProgram()->SetVec3("color", glm::vec3(0.0f, 0.0f, 0.0f));
	glBindVertexArray(mNormalVAO);
	glDrawArrays(GL_LINES, 0,m_InstanceSize * 2);
	glBindVertexArray(0);
	m_ContactsUpdated = false;
	}
}


void ContactsNode::PostRender(Scene* scene)
{

}

