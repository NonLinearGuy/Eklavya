#include "SkyNode.h"

#include "../Components/GameActor.h"
#include "../Components/Transform.h"
#include "../Components/BaseRenderComponent.h"
#include "../Helpers.h"
#include "DebugCamera.h"
#include "../Engine.h"
#include "Scene.h"
#include "../AssetManager/AssetManager.h"


SkyNode::SkyNode(ActorID id, BaseRenderComponent * renderComponent, ERenderGroup renderPass)
	:
	BaseNode(id,renderComponent,renderPass)
{
}

SkyNode::~SkyNode()
{
}

bool SkyNode::Init()
{

	float skyboxVertices[] = {
		// positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	m_VAO.Create(skyboxVertices,sizeof(skyboxVertices),0);
	m_VAO.SetPosPtr(3,0,0);

	m_Cubemap = AssetManager::GetInstance().GetAsset<Cubemap>("day");
	assert(m_Cubemap);

	return true;
}

void SkyNode::Destroy()
{
	
}

void SkyNode::PreRender(Scene * scene)
{
	std::shared_ptr<GameActor> gameActor = scene->GetEngineRef()->GetActor(m_ActorID);
	std::shared_ptr<Transform> transform = MakeSharedPtr(gameActor->GetComponent<Transform>(Transform::s_ID));
	glm::mat4 model = glm::mat4(1.0f);
	if (transform)
		model = transform->GetModelMatrix();
	scene->PushMatrix(model);
	m_Cubemap->BindToUnit(GL_TEXTURE0);
}

void SkyNode::Render(Scene * scene)
{
	m_VAO.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

