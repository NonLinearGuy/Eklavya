#include "BoxColliderNode.h"
#include "../Components/Transform.h"
#include "Scene.h"
#include <memory>
#include "../Components/GameActor.h"
#include "DebugCamera.h"
#include "../Engine.h"
#include "../Helpers.h"
#include "../Renderer/GLRenderer.h"
#include "../AssetManager/ShaderProgram.h"
#include "../Renderer/Material.h"
#include "BoundingVolume.h"
#include "../Renderer/VertexArrayObject.h"
#include "../Globals.h"
#include "../Engine.h"
#include "../Event/EventDispatcher.h"


BoxColliderNode::BoxColliderNode(ActorID actorID, const glm::vec3& extents)
	:
	BaseNode(actorID, nullptr, ERenderGroup::BOUND_VOLUME),
	m_Scale(glm::scale(glm::mat4(1.0f), extents))
{
}

BoxColliderNode::~BoxColliderNode()
{

}

bool BoxColliderNode::Init()
{

	m_CubeVAO = new CuboidVAO();
	m_CubeVAO->Init();

	m_Material = new Material();
	std::shared_ptr<GameActor> gameActor = g_Engine->GetActor(m_ActorID);
	m_Transform = gameActor->GetComponent<Transform>(Transform::s_ID);

	RegisterToEvent<BoxColliderNode>(this,&BoxColliderNode::OnCollisionEnter,EEventType::ON_COLLISION_ENTER);
	RegisterToEvent<BoxColliderNode>(this, &BoxColliderNode::OnCollisionExit, EEventType::ON_COLLISION_EXIT);
	
	return true;
}

void BoxColliderNode::Destroy()
{
	UnRegisterToEvent<BoxColliderNode>(this, &BoxColliderNode::OnCollisionEnter, EEventType::ON_COLLISION_ENTER);
	UnRegisterToEvent<BoxColliderNode>(this, &BoxColliderNode::OnCollisionExit, EEventType::ON_COLLISION_EXIT);

	delete m_Material;
	m_CubeVAO->Destroy();
}

void BoxColliderNode::SetColor(const glm::vec3& color)
{
	m_Material->SetOpacity(.3f);
	m_Material->SetDiffuse(color);
	m_Material->SetUseColor(true);
}

void BoxColliderNode::Tick(Scene* scene, float deltaTime)
{
}

void BoxColliderNode::PreRender(Scene* scene)
{
	scene->PushMatrix(MakeSharedPtr(m_Transform)->GetTRMatrix() * m_Scale);
	if (m_Material)
	{
		auto shader = scene->GetRenderer()->GetActiveProgram();
		m_Material->SetPropsInShader(shader);
	}
}

void BoxColliderNode::Render(Scene* scene)
{
	m_CubeVAO->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	RenderChildren(scene);
}

void BoxColliderNode::PostRender(Scene* scene)
{
	scene->PopMatrix();
}

void BoxColliderNode::OnCollisionEnter(std::shared_ptr<IEventData> data)
{
	unsigned actorID = std::static_pointer_cast<EventOnCollisionEnter>(data)->ActorID;
	if (actorID == m_ActorID)
		m_Material->SetDiffuse(glm::vec3(0.6f,0.0f,0.0f));
}

void BoxColliderNode::OnCollisionExit(std::shared_ptr<IEventData> data)
{
	unsigned actorID = std::static_pointer_cast<EventOnCollisionEnter>(data)->ActorID;
	if (actorID == m_ActorID)
		m_Material->SetDiffuse(glm::vec3(0.0f, 0.6f, 0.0f));
}

