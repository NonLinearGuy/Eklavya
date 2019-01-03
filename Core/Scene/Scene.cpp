#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../Engine.h"
#include "BaseNode.h"
#include "DebugCamera.h"
#include "../Renderer/GLRenderer.h"


Scene::Scene(HipHop::Engine* engineRef,std::shared_ptr<GLRenderer> renderer,const std::string& pName) : 
	m_Name(pName), 
	m_EngineRef(engineRef), 
	m_Renderer(renderer)
{
}

Scene::~Scene()
{
}

void Scene::Init()
{
	m_Camera = std::make_shared<DebugCamera>();
	
	float aspectRatio = m_EngineRef->GetCurrentContext()->GetAspectRatio();
	m_Projection = glm::perspective(45.0f,aspectRatio,0.1f,1000.0f);// gotta move projection to camera

	m_Groups.reserve(ERenderGroup::MAX);

	std::shared_ptr<BaseNode> staticGroup = std::make_shared<BaseNode>(-1, nullptr, ERenderGroup::SOLID);
	m_Groups.push_back(staticGroup);

	std::shared_ptr<BaseNode> dynamicGroup = std::make_shared<BaseNode>(-1, nullptr, ERenderGroup::OUTLINED);
	m_Groups.push_back(dynamicGroup);

	std::shared_ptr<BaseNode> skyGroup = std::make_shared<BaseNode>(-1, nullptr, ERenderGroup::SKYBOX);
	m_Groups.push_back(skyGroup);

	glm::mat4 id = glm::mat4(1.0f);
	m_MatrixStack.push(id);
}

void Scene::Destroy()
{
	for (unsigned int index = 0; index < m_Groups.size(); ++index)
		m_Groups[index]->Destroy();

	//Then destroy the scene
}

void Scene::Tick(float deltaTime)
{
	for (unsigned int index = 0; index < m_Groups.size(); ++index)
		m_Groups[index]->Tick(this,deltaTime);
}

void Scene::AddChild(std::shared_ptr<BaseNode> pNewNode)
{
	auto group = pNewNode->GetRenderGroup();
	if (group >= ERenderGroup::MIN && group < ERenderGroup::MAX && m_Groups[group])
		m_Groups[group]->AddChild(pNewNode);
}

void Scene::RemoveChild(std::shared_ptr<BaseNode> pNode)
{
	auto group = pNode->GetRenderGroup();
	if (group >= ERenderGroup::MIN && group < ERenderGroup::MAX && m_Groups[group])
		m_Groups[group]->RemoveChild(pNode);
}

void Scene::Render()
{
	glm::mat4 view = glm::mat4(1.0f);

	for (int groupIndex = 0; groupIndex < ERenderGroup::MAX; ++groupIndex)
	{
		switch (groupIndex)
		{
		case ERenderGroup::SOLID:
			m_Renderer->SetShaderProgram(EShaderProgram::SOLID);
			m_Renderer->SetProjectionMatrix(m_Projection);
			view = m_Camera->GetView();
			m_Renderer->SetViewMatrix(view);
			m_Groups[groupIndex]->Render(this);
			break;
		case ERenderGroup::OUTLINED:
			break;
		case ERenderGroup::SKYBOX:
			m_Renderer->SetShaderProgram(EShaderProgram::SKYBOX);
			m_Renderer->SetProjectionMatrix(m_Projection);
			view = m_Camera->GetView();
			m_Renderer->SetViewMatrix(view);
			m_Renderer->ChangeState(new SkyBoxState());
			m_Groups[groupIndex]->Render(this);
			m_Renderer->ChangeState(new DefaultState());
			break;
		}
	}
}

void Scene::PushMatrix(const glm::mat4 & model)
{
	glm::mat4 toWorld = model * m_MatrixStack.top();
	m_MatrixStack.push(toWorld);
	m_Renderer->SetWorldMatrix(toWorld);
}

