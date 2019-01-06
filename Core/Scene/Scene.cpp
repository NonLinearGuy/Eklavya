#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../Engine.h"
#include "BaseNode.h"
#include "DebugCamera.h"
#include "../Renderer/GLRenderer.h"
#include "FrustumNode.h"
#include "WaterNode.h"


Scene::Scene(Engine* engineRef,std::shared_ptr<GLRenderer> renderer,const std::string& pName) : 
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
	m_Frustum = std::make_shared<FrustumNode>(45.0f,aspectRatio,.1f,70.0f);
	m_Projection = glm::perspective(45.0f,aspectRatio,0.1f,1000.0f);// gotta move projection to camera
	m_WaterNode = std::make_shared<WaterNode>();
	m_WaterNode->Init();

	m_Groups.reserve(ERenderGroup::MAX);

	std::shared_ptr<BaseNode> staticGroup = std::make_shared<BaseNode>(-1, nullptr, ERenderGroup::SOLID);
	m_Groups.push_back(staticGroup);

	std::shared_ptr<BaseNode> dynamicGroup = std::make_shared<BaseNode>(-1, nullptr, ERenderGroup::OUTLINED);
	m_Groups.push_back(dynamicGroup);

	std::shared_ptr<BaseNode> waterGroup = std::make_shared<BaseNode>(-1, nullptr, ERenderGroup::WATER);
	m_Groups.push_back(waterGroup);

	std::shared_ptr<BaseNode> skyGroup = std::make_shared<BaseNode>(-1, nullptr, ERenderGroup::SKYBOX);
	m_Groups.push_back(skyGroup);

	glm::mat4 id = glm::mat4(1.0f);
	m_MatrixStack.push(id);

	m_LightSource.m_Ambient = glm::vec3(1.0f);
	m_LightSource.m_Diffuse = glm::vec3(.5f);
	m_LightSource.m_Specular = glm::vec3(1.0f);

	m_LightSource.m_Position = glm::vec3(glm::vec3(-150.0f, 100.0f, 100.0f));

	m_Groups[ERenderGroup::OUTLINED]->AddChild(m_Frustum);
	m_Groups[ERenderGroup::WATER]->AddChild(m_WaterNode);
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

void Scene::ShadowPassRender()
{
	m_Groups[ERenderGroup::SOLID]->Render(this);
}

void Scene::MainPassRender()
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::vec3 lightViewSpacePos;

	for (int groupIndex = 0; groupIndex < ERenderGroup::MAX; ++groupIndex)
	{
		switch (groupIndex)
		{
		case ERenderGroup::SOLID:
			m_Renderer->SetShaderProgram(EShaderProgram::SOLID);
			m_Renderer->SetProjectionMatrix(m_Projection);
			view = m_Camera->GetView();
			m_Renderer->SetViewMatrix(view);
			m_Renderer->SetShadowPassValues();
			lightViewSpacePos = glm::vec3(view * glm::vec4(m_LightSource.m_Position, 1.0f));
			m_Renderer->SetLightValues(lightViewSpacePos,&m_LightSource);
			m_Groups[groupIndex]->Render(this);
			break;
		case ERenderGroup::OUTLINED:
			m_Groups[ERenderGroup::OUTLINED]->Render(this);
			break;
		case ERenderGroup::WATER:
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			m_Groups[ERenderGroup::WATER]->Render(this);
			glDisable(GL_BLEND);
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

void Scene::WaterPassRender()
{
	glm::mat4 view(1.0f);

	for (int groupIndex = 0; groupIndex < ERenderGroup::MAX; ++groupIndex)
	{
		switch (groupIndex)
		{
		case ERenderGroup::SOLID:
			m_Renderer->SetProjectionMatrix(m_Projection);
			view = m_Camera->GetView();
			m_Renderer->SetViewMatrix(view);
			m_Groups[groupIndex]->Render(this);
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

void Scene::Render(ERenderPass pass)
{
	switch (pass)
	{
	case ERenderPass::SHADOW_PASS:
		m_ShouldSetMaterial = false;
		ShadowPassRender();
		break;
	case ERenderPass::MAIN_PASS:
		m_ShouldSetMaterial = true;
		MainPassRender();
		break;
	case ERenderPass::WATER_TEXTURE_PASS:
		m_ShouldSetMaterial = true;
		WaterPassRender();
		break;
	};
}

void Scene::PushMatrix(const glm::mat4 & model)
{
	glm::mat4 toWorld = m_MatrixStack.top() * model;
	m_MatrixStack.push(toWorld);
	m_Renderer->SetWorldMatrix(toWorld);
}

