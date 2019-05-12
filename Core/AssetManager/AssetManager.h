#pragma once

/* 
	Keeps all assets of different type in a map.
	different systems will ask for asset when required 
	will ownership of asset will be given to the system.
	The system which using the asset SHOULD GIVE UP ownership 
	when not using the asset so It can be destroyed by AssetManager in 
	the end.
*/

#include <string>
#include <map>
#include <list>
#include <memory>
#include <type_traits>

#include "IAsset.h"
#include "../Utils/Singleton.h"
#include <algorithm>
#include "Texture2D.h"
#include "ShaderProgram.h"
#include "Cubemap.h"
#include "Animation.h"
#include "Model.h"
#include "AssetFactory.h"
#include "../Utils/Logger.h"

using AssetList = std::list<std::shared_ptr<IAsset>>;

class AssetManager : public Singleton<AssetManager>
{
public:
	AssetManager();
	~AssetManager();
	
	void LoadTexture(std::string assetName, std::string ext, bool repeat = false);
	void LoadShader(std::string shaderName);
	void LoadCubemap(std::string folderName, std::string ext);
	void LoadModel(std::string modelName, std::string ext, int modelID);
	void LoadAnimation(std::string animName, std::string ext, int modelID);

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
		else if (std::is_same< Type, Model>::value)
			type = MODEL;
		else if (std::is_same< Type, Animation>::value)
			type = ANIMATION;
		

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
};