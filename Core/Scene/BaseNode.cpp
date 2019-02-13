#include "BaseNode.h"
#include "../Components/BaseComponent.h"
#include "../Components/BaseRenderComponent.h"
#include "../Components/Transform.h"
#include "../Components/GameActor.h"
#include "../Engine.h"
#include "../Helpers.h"
#include "../Renderer/Material.h"
#include "../Renderer/GLRenderer.h"
#include "DebugCamera.h"
#include "Frustum.h"
#include "BoundingVolume.h"


BaseNode::BaseNode(ActorID actorID,BaseRenderComponent* renderComponent,ERenderGroup renderPass)
	:
	m_ActorID(actorID),
	m_RenderGroup(renderPass),
	m_WeakRenderComponent(renderComponent),
	m_ToWorld(1.0f),
	m_BoundVolume(nullptr),
	m_Color(.7f),
	m_Opacity(1.0f)
{

}


BaseNode::~BaseNode()
{
	m_Children.clear();
}

bool BaseNode::Init()
{
	for (auto child : m_Children)
		child->Init();
	
	return true;
}

void BaseNode::Destroy()
{
	for (auto child : m_Children)
		child->Destroy();
}

void BaseNode::Tick(Scene* scene,float deltaTime)
{
	for (auto child : m_Children)
		child->Tick(scene,deltaTime);
}

void BaseNode::SetTransform(const glm::mat4& toWorld)
{
	m_ToWorld = toWorld;
}

void BaseNode::SetColor(glm::vec3 color)
{
	m_Material->SetDiffuse(color);
}

void BaseNode::PreRender(Scene *scene)
{
	std::shared_ptr<GameActor> gameActor = scene->GetEngineRef()->GetActor(m_ActorID);
	std::shared_ptr<Transform> transform = MakeSharedPtr(gameActor->GetComponent<Transform>(Transform::s_ID));
	if (transform)
	{
		m_ToWorld = transform->GetModelMatrix();
		if (m_BoundVolume)
		{
			std::static_pointer_cast<SphereBound>(m_BoundVolume)->SetCenter(transform->GetPosition());
		}
	}
	scene->PushMatrix(m_ToWorld);
	if ( m_Material && scene->ShouldSetMaterialProps() )
	{
		auto shader = scene->GetRenderer()->GetActiveProgram();
		m_Material->SetPropsInShader(shader);
	}
	
}

void BaseNode::Render(Scene* scene)
{
	RenderChildren(scene);
}

void BaseNode::RenderChildren(Scene* scene)
{
	for (auto child : m_Children)
	{
		child->PreRender(scene);
		if (scene->GetCamera()->GetFrustum()->IsInside(child->GetBoundVolume()))
		{
			child->Render(scene);
		}
		else
		{
			
		}
		child->PostRender(scene);
	}
}

void BaseNode::PostRender(Scene* scene)
{
	scene->PopMatrix();
}

void BaseNode::AddChild(std::shared_ptr<BaseNode> pChild)
{
	m_Children.push_back(pChild);
}

bool BaseNode::RemoveChild(std::shared_ptr<BaseNode> pChild)
{
	bool childRemoved = false;

	auto iter = std::find(m_Children.begin(), m_Children.end(),pChild);

	if (iter != m_Children.end())
	{
		iter = m_Children.erase(iter);
		childRemoved = true;
	}
	else
	{
		for (auto child : m_Children)
		{
			if (child->RemoveChild(pChild))
			{
				childRemoved = true;
				break;
			}
		}
	}

	return childRemoved;
}