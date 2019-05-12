#pragma once

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Helpers.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "../Utils/Logger.h"
#include "IAsset.h"
#include "ShaderProgram.h"

#define MAX_BONE_WEIGHTS 4

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	glm::vec2 TexCoords;
	float m_Weights[MAX_BONE_WEIGHTS];
	int m_BoneIDs[MAX_BONE_WEIGHTS];
};

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh 
{
public:
	Mesh(std::vector<Vertex> vertices, 
		std::vector<unsigned int> indices, 
		std::vector<Texture> textures);
	~Mesh();
	void DestroyBuffers();
	inline void SetName(const std::string& name) { m_Name = name; }
	void Render(std::shared_ptr<ShaderProgram> shader);
private:
	int m_IndicesSize;
	std::vector<Texture> m_Textures;
	GLuint m_VAO,m_VBO, m_EBO;
	std::string m_Name;
};




class Model : public IAsset
{
private:
	/* to make sure same texture won't be loaded more than once */
	std::vector<Texture> m_LoadedTextures;
	std::vector<Mesh> m_Meshes;
	std::string m_Directory;
public:
	
	unsigned m_ModelID;

public:
	Model(const std::string& assetName,int modelID);
	~Model();
	void Load(const std::string &fullPath);
	void Render(std::shared_ptr<ShaderProgram> shader);
private:
	void ProcessNode(aiNode *node, const aiScene *scene);
	void SetVertexBoneDataToDefault(Vertex& vertex);
	Mesh ExtractMeshData(aiMesh *mesh, const aiScene *scene);
	void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
	void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterial(aiMaterial *mat, 
										aiTextureType type,
											const std::string& typeName);
	unsigned int LoadTexture(const char *path, 
		const std::string &directory);
};