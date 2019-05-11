#include "AssetManager.h"

#include <cassert>


AssetManager::AssetManager()
	:
	Singleton()
{
	
}


AssetManager::~AssetManager()
{
	if (!m_AssetMap.empty())
		RemoveAll();
}

void AssetManager::LoadTexture(std::string assetName, std::string ext, bool repeat)
{
	auto newAsset = AssetFactory::CreateTexture(assetName,ext,repeat);
	m_AssetMap[EAssetType::TEXTURE].push_back(newAsset);
}

void AssetManager::LoadShader(std::string shaderName)
{
	auto newAsset = AssetFactory::CreateShader(shaderName);
	m_AssetMap[EAssetType::SHADER].push_back(newAsset);
}

void AssetManager::LoadCubemap(std::string folderName, std::string ext)
{
	auto newAsset = AssetFactory::CreateCubemap(folderName, ext);
	m_AssetMap[EAssetType::CUBEMAP].push_back(newAsset);
}

void AssetManager::LoadModel(std::string modelName, std::string ext, int modelID)
{
	auto newAsset = AssetFactory::CreateModel(modelName, ext, modelID);
	m_AssetMap[EAssetType::MODEL].push_back(newAsset);
}

void AssetManager::LoadAnimation(std::string animName, std::string ext, int modelID)
{
	auto newAsset = AssetFactory::CreateAnimation(animName, ext, modelID);
	m_AssetMap[EAssetType::ANIMATION].push_back(newAsset);
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
}

