#include "IAsset.h"
#include "../Utils/Logger.h"

IAsset::IAsset(EAssetType type, const std::string & name)
	:
	m_Type(type),
	m_Name(name)
{
	
}

IAsset::~IAsset()
{
	LOG_F("   %s Destroyed", m_Name.c_str());
}
