#include "BoxNode.h"
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
#include "../Components/BaseRenderComponent.h"
#include "../Renderer/VertexArrayObject.h"
#include "../Globals.h"
#include "../Engine.h"


BoxNode::BoxNode(ActorID actorID,BaseRenderComponent* renderComponent, ERenderGroup renderPass)
	:
	BaseNode(actorID,renderComponent,renderPass)
{

}

BoxNode::~BoxNode()
{

}

bool BoxNode::Init()
{
	
	m_CubeVAO = new CuboidVAO();
	m_CubeVAO->Init();

	m_Material = new Material();
	if (!m_AlbedoName.empty())
	{
		auto texture = std::make_shared<Texture2D>();
		texture->CreateTexture("Assets/Textures/" + m_AlbedoName);
		m_Material->SetAlbedoMap(texture);
		m_Material->SetDiffuse(glm::vec3(1.0f));
		m_Material->SetAmbient(glm::vec3(.5f));
		m_Material->SetUseColor(false);
	}
	else
	{
		m_Material->SetDiffuse(m_Color * glm::vec3(1.0f));
		m_Material->SetAmbient(m_Color * glm::vec3(.5f));
		m_Material->SetUseColor(true);
	}

	if (!m_NormalName.empty())
	{
		auto texture = std::make_shared<Texture2D>();
		texture->CreateTexture("Assets/Textures/" + m_NormalName);
		m_Material->SetNormalMap(texture);
		m_Material->SetDiffuse(glm::vec3(1.0f));
		m_Material->SetAmbient(glm::vec3(.5f));
		m_Material->SetUseColor(false);
	}

	m_Material->SetOpacity(m_Opacity);

	std::shared_ptr<GameActor> gameActor = g_Engine->GetActor(m_ActorID);
	std::shared_ptr<Transform> transform = MakeSharedPtr(gameActor->GetComponent<Transform>(Transform::s_ID));
	m_BoundVolume = std::make_shared<BoxBound>(transform->GetScale());

	return true;
}

void BoxNode::Destroy()
{
	delete m_Material;
	m_CubeVAO->Destroy();
}

void BoxNode::Tick(Scene* scene,float deltaTime)
{
}

void BoxNode::Render(Scene* scene)
{
	m_CubeVAO->Bind();
	glDrawArrays(GL_TRIANGLES,0,36);
	RenderChildren(scene);
}

