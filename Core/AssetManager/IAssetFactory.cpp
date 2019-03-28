#include "IAssetFactory.h"
#include "Texture2D.h"
#include "ShaderProgram.h"
#include "Cubemap.h"
#include <vector>


IAssetFactory::IAssetFactory(const std::string & pDirPath) 
	: 
	m_DirPath(pDirPath)
{
}

IAssetFactory::~IAssetFactory()
{

}


/* ======TEXTURE FACTORY =====*/

TextureFactory::TextureFactory(const std::string & dirPath)
	:
	IAssetFactory(dirPath)
{
}

TextureFactory::~TextureFactory()
{
}

std::shared_ptr<IAsset> TextureFactory::Create(const std::string & assetName)
{
	auto newTexture = std::make_shared<Texture2D>(assetName);
	newTexture->CreateTexture(m_DirPath + assetName + ".png",true);
	return newTexture;
}

// ==========SHADER FACTORY===========//
ShaderFactory::ShaderFactory(const std::string & dirPath)
	:
	IAssetFactory(dirPath)
{
}

ShaderFactory::~ShaderFactory()
{
}

std::shared_ptr<IAsset> ShaderFactory::Create(const std::string & assetName)
{
	auto shader = std::make_shared<ShaderProgram>(assetName);
	std::string suffix = "";

	for (int type = 0; type < EShaderType::SHADER_TYPE_MAX; ++type)
	{
		switch (type)
		{
		case EShaderType::VERTEX:
			suffix = "_vs.glsl";
			break;
		case EShaderType::FRAGMENT:
			suffix = "_fs.glsl";
			break;
		};

		shader->AddAndCompile(m_DirPath + assetName + suffix,(EShaderType)type);
	}

	shader->Build();

	return shader;
}



// ==============CUBEMAP FACTORY ==========//
CubemapFactory::CubemapFactory(const std::string & dirPath)
	:
	IAssetFactory(dirPath)
{
}

CubemapFactory::~CubemapFactory()
{
}

std::shared_ptr<IAsset> CubemapFactory::Create(const std::string & assetName)
{
	std::vector<std::string> faceNames;

	std::string ext = ".tga";

	faceNames.push_back(m_DirPath + assetName + "/right" + ext);
	faceNames.push_back(m_DirPath + assetName + "/left" + ext);
	faceNames.push_back(m_DirPath + assetName + "/top" + ext);
	faceNames.push_back(m_DirPath + assetName + "/bottom" + ext);
	faceNames.push_back(m_DirPath + assetName + "/back" + ext);
	faceNames.push_back(m_DirPath + assetName + "/front" + ext);

	auto cubeMap = std::make_shared<Cubemap>( assetName );
	cubeMap->Create(faceNames);

	return cubeMap;
}
