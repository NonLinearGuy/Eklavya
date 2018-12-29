#pragma once

#include<vector>
#include<glm/glm.hpp>
#include "VertexArrayObject.h"
#include "ShaderProgram.h"

class VertexData
{
	glm::vec3 position;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	void Init(std::vector<glm::vec3> pVertices,std::vector<unsigned int> pIndices);
	void Render();
	
protected:
	std::vector<glm::vec3> m_Vertices;
	std::vector<unsigned int> m_Indices;
	VertexArrayObject m_VAOConfig;
};

