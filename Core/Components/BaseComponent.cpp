#include"BaseComponent.h"

unsigned int BaseComponent::sCompCounter = 0;

BaseComponent::BaseComponent(const std::string & name)
	:
	m_Name(name)
{
	m_ID = sCompCounter++;
}

BaseComponent::~BaseComponent()
{
}
