#include"BaseComponent.h"
#include "GameActor.h"

BaseComponent::BaseComponent()
{
	
}

BaseComponent::~BaseComponent()
{
}

unsigned BaseComponent::GetOwnerID()
{
	return m_Owner->GetID();
}
