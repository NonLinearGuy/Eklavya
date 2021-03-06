#include "TransformNode.h"
#include "../Renderer/VertexArrayObject.h"
#include "Scene.h"
#include "../Components/GameActor.h"
#include "../Components/Transform.h"
#include "../Helpers.h"
#include "../Engine.h"
#include "../Renderer/GLRenderer.h"


TransformNode::TransformNode(ActorID actorID) :BaseNode(actorID, nullptr, ERenderGroup::OUTLINED)
{

}

TransformNode::~TransformNode()
{

}

bool TransformNode::Init() 
{

	float scale = 1.0f;
	float vertices[] = 
	{
		0.0f,0.0f,0.0f, 0.0f,1.0f,0.0f,
		0.0f,scale,0.0f, 0.0f,1.0f,0.0f,

		0.0f,0.0f,0.0f, 1.0f,0.0f,0.0f,
		scale,0.0f,0.0f, 1.0f,0.0f,0.0f,
		
		0.0f,0.0f,0.0f, 0.0f,0.0f,1.0f,
		0.0f,0.0f,scale, 0.0f,0.0f,1.0f
	};


	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float) * 6,0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float) * 3));

	return true;
}

void TransformNode::Destroy()
{
	glDeleteBuffers(1,&mVBO);
	glDeleteVertexArrays(1,&mVAO);
}

void TransformNode::PreRender(Scene* scene)
{
	std::shared_ptr<GameActor> gameActor = scene->GetEngineRef()->GetActor(m_ActorID);
	std::shared_ptr<Transform> transform = MakeSharedPtr(gameActor->GetComponent<Transform>(Transform::s_ID));

	float length = glm::length(scene->GetCamera()->GetPosition() - transform->GetPosition()) * .05f;
	if (length > 40.0f) length = 40.0f;
	m_ToWorld =  glm::scale(transform->GetTRMatrix(), glm::vec3(length));
	
	scene->PushMatrix(m_ToWorld);
}

void TransformNode::Render(Scene * scene)
{
	glBindVertexArray(mVAO);
	glDrawArrays(GL_LINES,0,6);
	glBindVertexArray(0);
}


void TransformNode::PostRender(Scene* scene)
{
	scene->PopMatrix();
}

