#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp>
#include<glad/glad.h>
#include "DebugCamera.h"
#include "../Engine.h"
#include "RootNode.h"
#include "../Renderer/GLRenderer.h"


Scene::Scene(HipHop::Engine* engineRef,const std::string& pName) : m_Name(pName), m_EngineRef(engineRef)
{
}

Scene::~Scene()
{
}

void Scene::Init(std::shared_ptr<GLRenderer> renderer)
{
	m_Renderer = renderer;
	m_Camera = std::make_shared<DebugCamera>();
	m_Root = std::make_shared<RootNode>();

	float aspectRatio = m_EngineRef->GetCurrentContext()->GetAspectRatio();
	m_Projection = glm::perspective(45.0f,aspectRatio,0.1f,1000.0f);
}

void Scene::Destroy()
{
	m_Root->Destroy();
	//Destroy Scene resources here
}

void Scene::Tick(float deltaTime)
{
	m_Root->Tick(this,deltaTime);
}

void Scene::AddChild(std::shared_ptr<BaseNode> pNewNode)
{
	pNewNode->Init();
	m_Root->AddChild(pNewNode);
}

void Scene::RemoveChild(std::shared_ptr<BaseNode> pNode)
{
	m_Root->RemoveChild(pNode);
}

void Scene::Render()
{
	m_Root->Render(this);
}

