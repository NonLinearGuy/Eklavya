#include "SphereNode.h"
#include <vector>
#include "../Components/GameActor.h"
#include "../Components/Transform.h"
#include "../Components/BaseRenderComponent.h"
#include "../Helpers.h"
#include "DebugCamera.h"
#include "../Engine.h"
#include "../Renderer/GLRenderer.h"
#include "../Renderer/Material.h"

#include "Scene.h"
#include "Frustum.h"
#include "BoundingVolume.h"
#include "../Renderer/VertexArrayObject.h"

const float PI = 3.1415926535f;


SphereNode::SphereNode(ActorID id, BaseRenderComponent * renderComponent, ERenderGroup renderPass)
	:
	BaseNode(id,renderComponent,renderPass),
	m_SphereVAO(new SphereVAO)
{
	
}

SphereNode::~SphereNode()
{
	delete m_Material;
	delete m_SphereVAO;
}

bool SphereNode::Init()
{
	if(m_WeakRenderComponent)
		m_Radius = (static_cast<SphereRenderComponent*>(m_WeakRenderComponent))->GetRadius();
	else
		m_Radius = 1;

	float sectorCount = (float)(static_cast<SphereRenderComponent*>(m_WeakRenderComponent))->GetSectors();
	float stackCount = (float)(static_cast<SphereRenderComponent*>(m_WeakRenderComponent))->GetStacks();

	m_SphereVAO->Init(m_Radius,stackCount,sectorCount);
	
	m_Material = new Material();
	m_FloorTexture = std::make_shared<Texture2D>();
	m_FloorTexture->CreateTexture("Assets/Textures/floor_albedo.png");
	m_Material->SetAlbedoMap(m_FloorTexture);
	m_Material->SetDiffuse(glm::vec3(1.0f,0.0f,0.0f));
	m_Material->SetAmbient(glm::vec3(.5f,0.0f,0.0f));
	m_Material->SetUseColor(true);

	m_BoundVolume = std::make_shared<SphereBound>(glm::vec3(0.0f),m_Radius);

	return true;
}

void SphereNode::Destroy()
{
	m_SphereVAO->Destroy();
	m_BoundVolume.reset();
}


void SphereNode::Render(Scene * scene)
{	
	m_SphereVAO->Bind();
	glDrawElements(GL_TRIANGLES,m_SphereVAO->GetIndicesSize(),GL_UNSIGNED_INT,0);
	RenderChildren(scene);
}