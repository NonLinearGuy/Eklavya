#include "BaseRenderComponent.h"
#include "../Scene/BoxNode.h"
#include "../Scene/SphereNode.h"
#include "../Scene/SkyNode.h"
#include "Transform.h"
#include "GameActor.h"
#include "../Helpers.h"
#include "../Scene/Scene.h"
#include "../Helpers.h"
#include "../Scene/BaseNode.h"

ComponentID BaseRenderComponent::s_ID = 1;
ComponentID BoxRenderComponent::s_ID = 2;
ComponentID SphereRenderComponent::s_ID = 3;
ComponentID SkyRenderComponent::s_ID = 4;
ComponentID BoxColliderRenderComponent::s_ID = 5;
ComponentID SphereColliderRenderComponent::s_ID = 6;


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


void BoxRenderComponent::CreateBaseNode()
{
	if (m_Owner)
	{
		auto sharedTransform = MakeSharedPtr(m_Owner->GetComponent<Transform>(Transform::s_ID));
		if (sharedTransform)
		{
			BaseRenderComponent* weakThis(this);
			m_BaseNode = std::make_shared<BoxNode>(m_Owner->GetID(), weakThis, ERenderGroup::SOLID);
		}
	}
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

void SphereRenderComponent::CreateBaseNode()
{
	if (m_Owner)
	{
		auto sharedTransform = MakeSharedPtr(m_Owner->GetComponent<Transform>(Transform::s_ID));
		if (sharedTransform)
		{
			BaseRenderComponent* weakThis(this);
			m_BaseNode = std::make_shared<SphereNode>(m_Owner->GetID(), weakThis, ERenderGroup::SOLID);
		}
	}
}



/*TO RENDER COLLIDER*/

/* BOX */
BoxColliderRenderComponent::BoxColliderRenderComponent()
	:
	BaseRenderComponent()
{

}

BoxColliderRenderComponent::~BoxColliderRenderComponent()
{

}


void BoxColliderRenderComponent::CreateBaseNode()
{
	if (m_Owner)
	{
		auto sharedTransform = MakeSharedPtr(m_Owner->GetComponent<Transform>(Transform::s_ID));
		if (sharedTransform)
		{
			BaseRenderComponent* weakThis(this);
			m_BaseNode = std::make_shared<BoxNode>(m_Owner->GetID(), weakThis, ERenderGroup::SOLID);
		}
	}
}



/* SPHERE */

SphereColliderRenderComponent::SphereColliderRenderComponent(float pRadius, int pStacks, int pSectors)
	:
	m_Stacks(pStacks),
	m_Sectors(pSectors),
	m_Radius(pRadius)
{

}

SphereColliderRenderComponent::~SphereColliderRenderComponent()
{

}

void SphereColliderRenderComponent::CreateBaseNode()
{
	if (m_Owner)
	{
		auto sharedTransform = MakeSharedPtr(m_Owner->GetComponent<Transform>(Transform::s_ID));
		if (sharedTransform)
		{
			BaseRenderComponent* weakThis(this);
			m_BaseNode = std::make_shared<SphereNode>(m_Owner->GetID(), weakThis, ERenderGroup::SOLID);
		}
	}
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

void SkyRenderComponent::CreateBaseNode()
{
	if (m_Owner)
	{
		auto sharedTransform = MakeSharedPtr(m_Owner->GetComponent<Transform>(Transform::s_ID));
		if (sharedTransform)
		{
			BaseRenderComponent* weakThis(this);
			m_BaseNode = std::make_shared<SkyNode>(m_Owner->GetID(), weakThis, ERenderGroup::SKYBOX);
		}
	}
}




