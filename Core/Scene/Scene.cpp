#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../Engine.h"
#include "BaseNode.h"
#include "DebugCamera.h"
#include "../Renderer/GLRenderer.h"
#include "Frustum.h"
#include "WaterNode.h"
#include "../Helpers.h"
#include "BoundingVolume.h"
#include "../Event/EventDispatcher.h"
#include "../Event/Events.h"
#include <functional>
#include "../Components/BaseRenderComponent.h"
#include "ArrowNode.h"


#define DRAW_TRANSFORM

Scene::Scene(Engine* engineRef,std::shared_ptr<GLRenderer> renderer,const std::string& pName) : 
	m_Name(pName), 
	m_EngineRef(engineRef), 
	m_Renderer(renderer)
{
	m_Nodes.reserve(100);
	m_Groups.reserve(ERenderGroup::MAX);

	std::shared_ptr<BaseNode> staticGroup = std::make_shared<BaseNode>(ACTOR_NOT_NEEDED, nullptr, ERenderGroup::SOLID);
	m_Groups.push_back(staticGroup);

	std::shared_ptr<BaseNode> waterGroup = std::make_shared<BaseNode>(ACTOR_NOT_NEEDED, nullptr, ERenderGroup::WATER);
	m_Groups.push_back(waterGroup);

	std::shared_ptr<BaseNode> outlinedGroup = std::make_shared<BaseNode>(ACTOR_NOT_NEEDED, nullptr, ERenderGroup::OUTLINED);
	m_Groups.push_back(outlinedGroup);

	std::shared_ptr<BaseNode> boundVolumeGroup = std::make_shared<BaseNode>(ACTOR_NOT_NEEDED, nullptr, ERenderGroup::BOUND_VOLUME);
	m_Groups.push_back(boundVolumeGroup);

	std::shared_ptr<BaseNode> skyGroup = std::make_shared<BaseNode>(ACTOR_NOT_NEEDED, nullptr, ERenderGroup::SKYBOX);
	m_Groups.push_back(skyGroup);

	glm::mat4 id = glm::mat4(1.0f);
	m_MatrixStack.push(id);

	RegisterToEvent<Scene>(this, &Scene::OnActorCreated, EEventType::ACTOR_CREATED);
	RegisterToEvent<Scene>(this, &Scene::OnActorDestroyed, EEventType::ACTOR_DESTROYED);
}

Scene::~Scene()
{
	UnRegisterToEvent<Scene>(this, &Scene::OnActorCreated, EEventType::ACTOR_CREATED);
	UnRegisterToEvent<Scene>(this, &Scene::OnActorDestroyed, EEventType::ACTOR_DESTROYED);
}

void Scene::Init()
{


	m_LightSource.m_Ambient = glm::vec3(1.0f);
	m_LightSource.m_Diffuse = glm::vec3(.8f);
	m_LightSource.m_Specular = glm::vec3(1.0f);

	m_LightSource.m_Position = glm::vec3(glm::vec3(-250.0f, 200.0f, 200.0f));

	float aspectRatio = m_EngineRef->GetCurrentContext()->GetAspectRatio();
	m_Camera = std::make_shared<DebugCamera>(45.0f, aspectRatio, 0.1f, 10000.0f);
	m_Camera->Init();
	m_WaterNode = std::make_shared<WaterNode>();
	m_WaterNode->Init();

	AddChild(m_WaterNode);
	AddChild(m_Camera);

	
}

void Scene::OnActorCreated(std::shared_ptr<IEventData> data)
{
	auto actor = std::static_pointer_cast<EventActorCreated>(data)->m_Actor;
	auto baseNode = MakeSharedPtr(actor->GetComponent<BaseRenderComponent>(BaseRenderComponent::s_ID))->GetBaseNode();
	baseNode->Init();
	AddChild(baseNode);

#ifdef DRAW_TRANSFORM
	if (baseNode->GetRenderGroup() != ERenderGroup::SKYBOX)
	{
		auto arrowNode = std::make_shared<ArrowNode>(actor->GetID());
		arrowNode->Init();
		AddChild(arrowNode);
	}
#endif
	
}


void Scene::OnActorDestroyed(std::shared_ptr<IEventData> data)
{
	auto actor = std::static_pointer_cast<EventActorCreated>(data)->m_Actor;
	auto baseNode = MakeSharedPtr(actor->GetComponent<BaseRenderComponent>(BaseRenderComponent::s_ID))->GetBaseNode();
	RemoveChild(baseNode);
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
	{
		m_Nodes.push_back(pNewNode);
		m_Groups[group]->AddChild(pNewNode);
	}
}


void Scene::RemoveChild(std::shared_ptr<BaseNode> pNode)
{
	bool removed = false;

	auto iter = std::find(m_Nodes.begin() , m_Nodes.end() , pNode); /*first check if its in the tree, 
																	if yes then remove it from it*/
	if (iter != m_Nodes.end())
	{
		iter = m_Nodes.erase(iter);
		auto group = pNode->GetRenderGroup();
		removed = m_Groups[group]->RemoveChild(pNode);
		assert(removed);
	}
}

void Scene::ShadowPassRender()
{
	m_Groups[ERenderGroup::SOLID]->Render(this);
}

void Scene::MainPassRender()
{
	glm::mat4 view = m_Camera->GetView();
	glm::mat4 projection = m_Camera->GetProjection();
	glm::vec3 lightViewSpacePos = glm::vec3(view * glm::vec4(m_LightSource.m_Position, 1.0f));	

	for (int groupIndex = 0; groupIndex < ERenderGroup::MAX; ++groupIndex)
	{
		switch (groupIndex)
		{
		case ERenderGroup::SOLID:
			m_Renderer->SetShaderProgram(EShaderProgram::SOLID);
			m_Renderer->SetProjectionMatrix(projection);
			m_Renderer->SetViewMatrix(view);
			m_Renderer->SetShadowPassValues();
			m_Renderer->SetLightValues(lightViewSpacePos,&m_LightSource);
			m_Groups[ERenderGroup::SOLID]->Render(this);
			break;
		case ERenderGroup::WATER:
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			m_Groups[ERenderGroup::WATER]->Render(this);
			glDisable(GL_BLEND);
			break;
		case ERenderGroup::BOUND_VOLUME:
			m_Renderer->SetShaderProgram(EShaderProgram::UNLIT_SOLID);
			m_Renderer->SetProjectionMatrix(projection);
			view = m_Camera->GetView();
			m_Renderer->SetViewMatrix(view);
			m_Groups[ERenderGroup::BOUND_VOLUME]->Render(this);
			break;
		case ERenderGroup::OUTLINED:
			glDepthFunc(GL_ALWAYS);
			glEnable(GL_LINE_WIDTH);
			glLineWidth(3.0f);
			m_Renderer->SetShaderProgram(EShaderProgram::OUTLINED);
			m_Renderer->SetProjectionMatrix(projection);
			m_Renderer->SetViewMatrix(view);
			m_Groups[ERenderGroup::OUTLINED]->Render(this);
			glDisable(GL_LINE_WIDTH);
			glDepthFunc(GL_LEQUAL);
			break;
		case ERenderGroup::SKYBOX:
			m_Renderer->SetShaderProgram(EShaderProgram::SKYBOX);
			m_Renderer->SetProjectionMatrix(projection);
			view = m_Camera->GetView();
			m_Renderer->SetViewMatrix(view);
			m_Renderer->ChangeState(new SkyBoxState());
			m_Groups[ERenderGroup::SKYBOX]->Render(this);
			m_Renderer->ChangeState(new DefaultState());
			break;
		}
	}
}

void Scene::WaterPassRender()
{
	glm::mat4 view = m_Camera->GetView();
	glm::mat4 projection = m_Camera->GetProjection();

	
	for (int groupIndex = 0; groupIndex < ERenderGroup::MAX; ++groupIndex)
	{
		switch (groupIndex)
		{
		case ERenderGroup::SOLID:
			m_Renderer->SetProjectionMatrix(projection);
			view = m_Camera->GetView();
			m_Renderer->SetViewMatrix(view);
			m_Groups[groupIndex]->Render(this);
			break;
		case ERenderGroup::SKYBOX:
			m_Renderer->SetShaderProgram(EShaderProgram::SKYBOX);
			m_Renderer->SetProjectionMatrix(projection);
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

