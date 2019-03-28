#pragma once

#include <string>
#include <map>
#include <list>
#include <memory>
#include <typeinfo>
#include <type_traits>

#include "IAsset.h"
#include "../Utils/Singleton.h"
#include <algorithm>
#include "Texture2D.h"
#include "ShaderProgram.h"
#include "Cubemap.h"

using AssetList = std::list<std::shared_ptr<IAsset>>;

class IAssetFactory;

class AssetManager : public Singleton<AssetManager>
{
public:
	AssetManager();
	~AssetManager();
	void LoadAsset(EAssetType type,const std::string& name);

	template<typename Type>
	std::shared_ptr<Type> GetAsset(const std::string& name)
	{
		EAssetType type = ASSET_TYPE_MAX;

		if ( std::is_same< Type,Texture2D>::value )
			type = TEXTURE;
		else if ( std::is_same< Type, ShaderProgram>::value )
			type = SHADER;
		else if ( std::is_same< Type, Cubemap>::value )
			type = CUBEMAP;

		assert(type != ASSET_TYPE_MAX);

		auto listEnd = m_AssetMap[type].end();
		auto iter = std::find_if(m_AssetMap[type].begin(), listEnd,
			[&](std::shared_ptr<IAsset> asset)
		{
			return name == asset->GetName();
		}
		);

		if (iter != listEnd)
			return std::static_pointer_cast<Type>(*iter);
		else
			return std::shared_ptr<Type>(nullptr);
	}

	void RemoveAsset(EAssetType type,const std::string& name);
	void RemoveAll();

private:

	std::map<EAssetType, AssetList> m_AssetMap;
	
	IAssetFactory* m_AssetFactories[ASSET_TYPE_MAX];

	static const std::string s_TexturesDirPath;
	static const std::string s_CubemapsDirPath;
	static const std::string s_ShadersDirPath;

};