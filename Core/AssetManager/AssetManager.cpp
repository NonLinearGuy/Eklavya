#include "AssetManager.h"
#include "../Utils/Logger.h"
#include "IAssetFactory.h"
#include <cassert>


const std::string AssetManager::s_TexturesDirPath = "Assets/Textures/";
const std::string AssetManager::s_CubemapsDirPath = "Assets/Skybox/";
const std::string AssetManager::s_ShadersDirPath = "Assets/Shaders/";


AssetManager::AssetManager()
	:
	Singleton()
{
	for (int type = ASSET_TYPE_MIN; type < ASSET_TYPE_MAX; ++type)
	{
		switch (type)
		{
		case EAssetType::TEXTURE:
			m_AssetFactories[type] = new TextureFactory(s_TexturesDirPath);
			break;
		case EAssetType::SHADER:
			m_AssetFactories[type] = new ShaderFactory(s_ShadersDirPath);
			break;
		case EAssetType::CUBEMAP:
			m_AssetFactories[type] = new CubemapFactory(s_CubemapsDirPath);
			break;
		}
	}
}


AssetManager::~AssetManager()
{
	if (!m_AssetMap.empty())
		RemoveAll();
}

void AssetManager::LoadAsset(EAssetType type, const std::string & name)
{
	assert( (type >= ASSET_TYPE_MIN && type < ASSET_TYPE_MAX ) );

	auto listEnd = m_AssetMap[type].end();
	auto iter = std::find_if(m_AssetMap[type].begin(), listEnd,
		[&](std::shared_ptr<IAsset> asset)
	{
		return name == asset->GetName();
	}
	);

	if (iter == listEnd)
	{
		auto newAsset = m_AssetFactories[type]->Create(name);
		m_AssetMap[type].push_back(newAsset);
		LOG_STRING(("    " + name + " Loaded"));
	}
}

void AssetManager::RemoveAsset(EAssetType type,const std::string & name)
{
	auto listEnd = m_AssetMap[type].end();
	auto iter = std::find_if(m_AssetMap[type].begin(), listEnd,
		[&](std::shared_ptr<IAsset> asset)
	{

		return ( name == asset->GetName() );
	}
	);
	if(iter != listEnd)
		m_AssetMap[type].erase(iter);
}

void AssetManager::RemoveAll()
{
	for (int type = ASSET_TYPE_MIN; type < ASSET_TYPE_MAX; ++type)
	{
		auto& list = m_AssetMap[(EAssetType)type];
		for (auto& assetPtr : list)
			assetPtr.reset(); // will call respective destructors
		list.clear();
	}
	m_AssetMap.clear();
	int a;
}

