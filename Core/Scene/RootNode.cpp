#include "RootNode.h"
#include "Scene.h"
#include "../Renderer/GLRenderer.h"

RootNode::RootNode()
	:
	BaseNode(-1,nullptr,ERenderGroup::MIN) //-1 is invalid actor
{
	m_Children.reserve(ERenderGroup::MAX);

	std::shared_ptr<BaseNode> staticGroup = std::make_shared<BaseNode>(-1,nullptr,ERenderGroup::STATIC);
	m_Children.push_back(staticGroup);

	std::shared_ptr<BaseNode> dynamicGroup = std::make_shared<BaseNode>(-1,nullptr,ERenderGroup::DYNAMIC);
	m_Children.push_back(dynamicGroup);

	std::shared_ptr<BaseNode> skyGroup = std::make_shared<BaseNode>(-1,nullptr,ERenderGroup::SKYBOX);
	m_Children.push_back(skyGroup);
}

RootNode::~RootNode()
{
	
}

void RootNode::AddChild(std::shared_ptr<BaseNode> pChild)
{
	ERenderGroup pass = pChild->GetRenderPass();
	if (pass < ERenderGroup::MAX && m_Children[pass])
	{
		m_Children[pass]->AddChild(pChild);
	}
}

bool RootNode::RemoveChild(std::shared_ptr<BaseNode> m_Child)
{
	bool childRemoved = false;

	for (int pass = 0; pass < ERenderGroup::MAX; ++pass)
	{
		if (m_Children[pass]->RemoveChild(m_Child))
		{
			childRemoved = true;
		}
	}

	return childRemoved;
}

void RootNode::RenderChildren(Scene* scene)
{
	for (int pass = 0; pass < ERenderGroup::MAX; ++pass)
	{
		switch (pass)
		{
		case ERenderGroup::STATIC:
		case ERenderGroup::DYNAMIC:
			m_Children[pass]->RenderChildren(scene);
			break;

		case ERenderGroup::SKYBOX:
			scene->GetRenderer()->ChangeState(new SkyBoxState());
			m_Children[pass]->RenderChildren(scene);
			scene->GetRenderer()->ChangeState(new DefaultState());
			//exit render state
			break;
		}
	}
}
