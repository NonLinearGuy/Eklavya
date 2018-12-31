#include "BaseRenderComponent.h"
#include "../Scene/BoxNode.h"
#include "../Scene/SphereNode.h"
#include "../Scene/SkyNode.h"
#include "Transform.h"
#include "GameActor.h"
#include "../Helpers.h"

ComponentID BaseRenderComponent::s_ID = 1;
ComponentID BoxRenderComponent::s_ID = 2;
ComponentID SphereRenderComponent::s_ID = 3;
ComponentID SkyRenderComponent::s_ID = 4;


//Base
BaseRenderComponent::BaseRenderComponent()
	:
	BaseComponent()
{
}

BaseRenderComponent::~BaseRenderComponent()
{
}







/* BOX */
BoxRenderComponent::BoxRenderComponent()
	:
	BaseRenderComponent()
{

}

BoxRenderComponent::~BoxRenderComponent()
{

}


std::shared_ptr<BaseNode> BoxRenderComponent::CreateBaseNode()
{
	if (m_Owner)
	{
		auto sharedTransform = MakeSharedPtr(m_Owner->GetComponent<Transform>(Transform::s_ID));
		if (sharedTransform)
		{
			BaseRenderComponent* weakThis(this);
			auto boxNode = std::make_shared<BoxNode>(m_Owner->GetID(),weakThis,ERenderPass::DYNAMIC);
			return boxNode;
		}
		else
			return std::shared_ptr<BaseNode>();
	}
	return std::shared_ptr<BaseNode>();
}



/* SPHERE */

SphereRenderComponent::SphereRenderComponent(float pRadius, int pStacks, int pSectors )
	:
	BaseRenderComponent(),
	m_Stacks(pStacks),
	m_Sectors(pSectors),
	m_Radius(pRadius)
{

}

SphereRenderComponent::~SphereRenderComponent()
{

}

std::shared_ptr<BaseNode> SphereRenderComponent::CreateBaseNode()
{
	if (m_Owner)
	{
		auto sharedTransform = MakeSharedPtr(m_Owner->GetComponent<Transform>(Transform::s_ID));
		if (sharedTransform)
		{
			BaseRenderComponent* weakThis(this);
			auto boxNode = std::make_shared<SphereNode>(m_Owner->GetID(), weakThis, ERenderPass::DYNAMIC);
			return boxNode;
		}
		else
			return std::shared_ptr<BaseNode>();
	}
	return std::shared_ptr<BaseNode>();
}



/* Sky */


SkyRenderComponent::SkyRenderComponent()
	:
	BaseRenderComponent()
{

}

SkyRenderComponent::~SkyRenderComponent()
{

}

std::shared_ptr<BaseNode> SkyRenderComponent::CreateBaseNode()
{
	if (m_Owner)
	{
		auto sharedTransform = MakeSharedPtr(m_Owner->GetComponent<Transform>(Transform::s_ID));
		if (sharedTransform)
		{
			BaseRenderComponent* weakThis(this);
			auto boxNode = std::make_shared<SkyNode>(m_Owner->GetID(), weakThis, ERenderPass::SKYBOX);
			return boxNode;
		}
		else
			return std::shared_ptr<BaseNode>();
	}
	return std::shared_ptr<BaseNode>();
}




