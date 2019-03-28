#include "BoxBoundNode.h"
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


BoxBoundNode::BoxBoundNode(ActorID actorID,const glm::vec3& extents)
	:
	BaseNode(actorID,nullptr,ERenderGroup::BOUND_VOLUME),
	m_Scale(glm::scale(glm::mat4(1.0f),extents))
{
}

BoxBoundNode::~BoxBoundNode()
{

}

bool BoxBoundNode::Init()
{

	m_CubeVAO = new CuboidVAO();
	m_CubeVAO->Init();

	m_Material = new Material();
	std::shared_ptr<GameActor> gameActor = g_Engine->GetActor(m_ActorID);
	m_Transform = gameActor->GetComponent<Transform>(Transform::s_ID);

	return true;
}

void BoxBoundNode::Destroy()
{
	delete m_Material;
	m_CubeVAO->Destroy();
}

void BoxBoundNode::SetColor(const glm::vec3& color)
{
	m_Material->SetOpacity(.3f);
	m_Material->SetDiffuse(color);
	m_Material->SetUseColor(true);
}

void BoxBoundNode::Tick(Scene* scene, float deltaTime)
{
}

void BoxBoundNode::PreRender(Scene* scene)
{
	scene->PushMatrix(MakeSharedPtr(m_Transform)->GetTRMatrix() * m_Scale);
	if (m_Material)
	{
		auto shader = scene->GetRenderer()->GetActiveProgram();
		m_Material->SetPropsInShader(shader);
	}
}

void BoxBoundNode::Render(Scene* scene)
{
	m_CubeVAO->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	RenderChildren(scene);
}

void BoxBoundNode::PostRender(Scene* scene)
{
	scene->PopMatrix();
}

