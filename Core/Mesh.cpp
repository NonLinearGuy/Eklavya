#include "Mesh.h"



Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

void Mesh::Init(std::vector<glm::vec3> pVertices, std::vector<unsigned int> pIndices)
{
	m_Vertices = pVertices;
	m_Indices = pIndices;

	m_VAOConfig.Create((float*)&m_Vertices[0],sizeof(glm::vec3) * m_Vertices.size(),(int*)&m_Indices[0],sizeof(unsigned int) * m_Indices.size());
	m_VAOConfig.SetPosPtr(3,0,0);
}

void Mesh::Render()
{
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINES);
	m_VAOConfig.Bind();
	glDrawElements(GL_TRIANGLES,m_Indices.size(), GL_UNSIGNED_INT,0);
	m_VAOConfig.Unbind();
}
