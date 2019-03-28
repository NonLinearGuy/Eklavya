#include "IAsset.h"

IAsset::IAsset(EAssetType type, const std::string & name)
	:
	m_Type(type),
	m_Name(name)
{
}
