#pragma once

#include<vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include"Mesh.h"
#include <string>
#include "ShaderProgram.h"

namespace HipHop
{
	class Model
	{
	public:
		Model();

		bool Load(const std::string& path);
		void Render();
		void SetShaderValues();
		inline ShaderProgram GetShader() { return m_Shader; }
		~Model();
	private:

		void CreateMesh(aiMesh* mesh,const aiScene* scene);
		std::string m_OptDirectory;
		ShaderProgram m_Shader;
		std::vector<Mesh> m_Meshes;

	};
}

