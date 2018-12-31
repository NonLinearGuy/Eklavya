#include "RootNode.h"

RootNode::RootNode()
	:
	BaseNode(-1,nullptr,ERenderPass::MIN) //-1 is invalid actor
{
	m_Children.reserve(ERenderPass::MAX);

	std::shared_ptr<BaseNode> staticGroup = std::make_shared<BaseNode>(-1,nullptr,ERenderPass::STATIC);
	m_Children.push_back(staticGroup);

	std::shared_ptr<BaseNode> dynamicGroup = std::make_shared<BaseNode>(-1,nullptr,ERenderPass::DYNAMIC);
	m_Children.push_back(dynamicGroup);

	std::shared_ptr<BaseNode> skyGroup = std::make_shared<BaseNode>(-1,nullptr,ERenderPass::SKYBOX);
	m_Children.push_back(skyGroup);
}

RootNode::~RootNode()
{
	
}

void RootNode::AddChild(std::shared_ptr<BaseNode> pChild)
{
	ERenderPass pass = pChild->GetRenderPass();
	if (pass < ERenderPass::MAX && m_Children[pass])
	{
		m_Children[pass]->AddChild(pChild);
	}
}

bool RootNode::RemoveChild(std::shared_ptr<BaseNode> m_Child)
{
	bool childRemoved = false;

	for (int pass = 0; pass < ERenderPass::MAX; ++pass)
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
	for (int pass = 0; pass < ERenderPass::MAX; ++pass)
	{
		switch (pass)
		{
		case ERenderPass::STATIC:
		case ERenderPass::DYNAMIC:
			m_Children[pass]->RenderChildren(scene);
			break;

		case ERenderPass::SKYBOX:
			//enter skybox render state
			m_Children[pass]->RenderChildren(scene);
			//exit render state
			break;
		}
	}
}
