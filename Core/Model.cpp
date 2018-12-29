#include "Model.h"
#include "Logger.h"
#include "Camera.h"

namespace HipHop
{
	Model::Model()
	{
	}


	Model::~Model()
	{
	}


	bool Model::Load(const std::string& path)
	{
		Assimp::Importer import;
		const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::string errorString = "ASSIMP ERROR ::" + std::string(import.GetErrorString());
			HipHop::Logger::GetInstance()->Log(errorString.c_str());
			return false;
		}
		m_OptDirectory = path.substr(0, path.find_last_of('/'));

		unsigned int meshSize = 0;

		for (int index = 0; index < scene->mNumMeshes; ++index)
		{
			aiMesh* mesh = scene->mMeshes[index];
			CreateMesh(mesh,scene);
		}

		m_Shader.AddAndCompile("test_vs.glsl",EShaderType::VERTEX);
		m_Shader.AddAndCompile("test_fs.glsl",EShaderType::FRAGMENT);
		m_Shader.Build();

		return true;
	}
	
	void Model::CreateMesh(aiMesh* meshData,const aiScene* scene)
	{
		std::vector<glm::vec3> vertices;
		Mesh mesh;

		for (int index = 0; index < meshData->mNumVertices; ++index)
		{
			glm::vec3 vertex;
			vertex.x = meshData->mVertices[index].x;
			vertex.y = meshData->mVertices[index].y;
			vertex.z = meshData->mVertices[index].z;
			vertices.push_back(vertex);
		}
		

		std::vector<unsigned int> indices;

		for (unsigned int i = 0; i < meshData->mNumFaces; i++)
		{
			aiFace face = meshData->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		
		mesh.Init(vertices,indices);
		m_Meshes.push_back(mesh);
	}

	void Model::SetShaderValues()
	{
		
	}

	void Model::Render()
	{
		m_Shader.Use();
		
		//glPolygonMode(GL_FRONT_AND_BACK,GL_LINES);
		for (auto mesh : m_Meshes)
			mesh.Render();
	}


}