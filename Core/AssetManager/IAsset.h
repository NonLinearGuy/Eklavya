#ifndef _INCLUDE_IASSET_H_
#define _INCLUDE_IASSET_H_

#include <string>

enum EAssetType
{
	ASSET_TYPE_MIN = 0,
	TEXTURE = ASSET_TYPE_MIN,
	SHADER,
	CUBEMAP,
	ASSET_TYPE_MAX
};

class IAsset
{
public:
	IAsset() = default; 
	IAsset(EAssetType type, const std::string& name);
	virtual ~IAsset();

	inline EAssetType GetType() const { return m_Type; }
	inline std::string GetName() const { return m_Name; }

protected:
	EAssetType m_Type;
	std::string m_Name;
};

#endif