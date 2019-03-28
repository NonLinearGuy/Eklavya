#pragma once

#include<string>

class IAsset;

class IAssetFactory
{
public:
	IAssetFactory(const std::string& pDirPath);
	virtual ~IAssetFactory();
	virtual std::shared_ptr<IAsset> Create(const std::string& name) = 0;
protected:
	std::string m_DirPath;
};


class TextureFactory : public IAssetFactory
{
public:
	TextureFactory(const std::string& dirPath);
	~TextureFactory()override;
	std::shared_ptr<IAsset> Create(const std::string& assetName) override;
};

class ShaderFactory : public IAssetFactory
{
public:
	ShaderFactory(const std::string& dirPath);
	~ShaderFactory()override;
	std::shared_ptr<IAsset> Create(const std::string& assetName) override;
};

class CubemapFactory : public IAssetFactory
{
public:
	CubemapFactory(const std::string& dirPath);
	~CubemapFactory()override;
	std::shared_ptr<IAsset> Create(const std::string& assetName) override;
};