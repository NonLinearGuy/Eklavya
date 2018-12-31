#include "BaseNode.h"
#include "../Components/BaseComponent.h"
#include "../Components/BaseRenderComponent.h"
#include "../Components/Transform.h"
#include "../Scene/Scene.h"
#include "../Components/GameActor.h"
#include "../Engine.h"

BaseNode::BaseNode(ActorID actorID,BaseRenderComponent* renderComponent,ERenderPass renderPass)
	:
	m_ActorID(actorID),
	m_RenderPass(renderPass),
	m_WeakRenderComponent(renderComponent)
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

void BaseNode::PreRender(Scene *scene)
{
	
}

void BaseNode::Render(Scene* scene)
{
	RenderChildren(scene);
}

void BaseNode::RenderChildren(Scene* scene)
{
	for (auto child : m_Children)
	{
		if (child->IsInsideFrustum(scene))
		{
			child->PreRender(scene);
			child->Render(scene);
			child->PostRender(scene);
		}
	}
}

void BaseNode::PostRender(Scene* scene)
{
	
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


