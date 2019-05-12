#include "AssetFactory.h"
#include "Texture2D.h"
#include "ShaderProgram.h"
#include "Cubemap.h"
#include "Model.h"
#include "Animation.h"
#include <vector>

const std::string AssetFactory::s_TexturesDirPath = "Assets/Textures/";
const std::string AssetFactory::s_CubemapsDirPath = "Assets/Skybox/";
const std::string AssetFactory::s_ShadersDirPath = "Assets/Shaders/";
const std::string AssetFactory::s_ModelsDirPath = "Assets/Models/";
const std::string AssetFactory::s_AnimationsDirPath = "Assets/Animations/";

std::shared_ptr<IAsset> AssetFactory::CreateTexture(std::string assetName, std::string ext, bool repeat)
{
	auto newTexture = std::make_shared<Texture2D>(assetName);
	newTexture->CreateTexture(s_TexturesDirPath + assetName + "." + ext, repeat);
	return newTexture;
}

std::shared_ptr<IAsset> AssetFactory::CreateShader(std::string assetName)
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

		shader->AddAndCompile(s_ShadersDirPath + assetName + suffix, (EShaderType)type);
	}

	shader->Build();

	return shader;
}

std::shared_ptr<IAsset> AssetFactory::CreateCubemap(std::string folderName, std::string ext)
{
	std::vector<std::string> faceNames;

	faceNames.push_back(s_CubemapsDirPath + folderName + "/right." + ext);
	faceNames.push_back(s_CubemapsDirPath + folderName + "/left." + ext);
	faceNames.push_back(s_CubemapsDirPath + folderName + "/top." + ext);
	faceNames.push_back(s_CubemapsDirPath + folderName + "/bottom." + ext);
	faceNames.push_back(s_CubemapsDirPath + folderName + "/back." + ext);
	faceNames.push_back(s_CubemapsDirPath + folderName + "/front." + ext);

	auto cubeMap = std::make_shared<Cubemap>(folderName);
	cubeMap->Create(faceNames);

	return cubeMap;
}

std::shared_ptr<IAsset> AssetFactory::CreateModel(std::string assetName, std::string ext, int modelID)
{
	auto newModel = std::make_shared<Model>(assetName,modelID);
	newModel->Load(s_ModelsDirPath + assetName + ext);
	return newModel;
}

std::shared_ptr<IAsset> AssetFactory::CreateAnimation(std::string assetName, std::string ext, int modelID)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(s_AnimationsDirPath + assetName + ext,aiProcess_Triangulate);

	assert(scene && scene->mRootNode);

	auto newAnimation = std::make_shared<Animation>(assetName,modelID,
		scene->mAnimations[0], scene);
	return newAnimation;
}
