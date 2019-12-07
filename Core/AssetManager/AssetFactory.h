#pragma once

#include <string>
#include <memory>

class IAsset;

class AssetFactory
{
public:
	static std::shared_ptr< IAsset > CreateTexture(std::string assetName,
		std::string ext, bool repeat = false);
	static std::shared_ptr< IAsset > CreateShader(std::string assetName);
	static std::shared_ptr< IAsset > CreateCubemap(std::string folderName,
		std::string ext);
	static std::shared_ptr< IAsset > CreateModel(std::string assetName,
		std::string ext,
		int modelID);/*model id is used to read/write to
					  AnimationData which is shared by animation and model */
	static std::shared_ptr< IAsset > CreateAnimation(std::string assetName,
		std::string ext,
		int modelID);

	static const std::string s_TexturesDirPath;
	static const std::string s_CubemapsDirPath;
	static const std::string s_ShadersDirPath;
	static const std::string s_ModelsDirPath;
	static const std::string s_AnimationsDirPath;

};