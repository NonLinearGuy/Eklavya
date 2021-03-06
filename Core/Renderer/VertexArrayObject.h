#ifndef _INC_HIP_HOP_VAO_H_
#define _INC_HIP_HOP_VAO_H_

#include<glad/glad.h>

class VertexArrayObject
{
public:
	VertexArrayObject(); 
	~VertexArrayObject();

	template<typename Type = float>
	void Create(Type* bufferData,size_t bufferDataSize,int* Indices, size_t indicesSize = 0);
	void Destroy();

	void SetPosPtr(int size,GLsizei startOffset, GLsizei strideOffset);
	void SetNormalPtr(GLsizei startOffset,GLsizei strideOffset);
	void SetTexPtr(GLsizei startOffset, GLsizei strideOffset);

	virtual void Bind() 
	{
		glBindVertexArray(m_VAO); 
	}

	virtual void Unbind()
	{
		glBindVertexArray(0);
	}
private:
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;
};


template<typename Type>
void VertexArrayObject::Create(Type* bufferData, size_t bufferDataSize, int* indices, size_t indicesSize)
{
	m_VBO = -1;
	m_EBO = -1;
	m_VAO = -1;

	glGenVertexArrays(1, &m_VAO);

	Bind();

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, bufferDataSize, bufferData, GL_STATIC_DRAW);

	if (indicesSize != 0)
	{
		glGenBuffers(1, &m_EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);
	}

	Unbind();
}


class SphereVAO
{
public:
	SphereVAO();
	~SphereVAO();
	int GetIndicesSize() { return m_IndicesSize; }
	void Init(float radius, int stack, int sector);
	void Destroy();
	void Bind();
	void Unbind();
private:
	VertexArrayObject m_VAO;
	int m_IndicesSize;
};

class CuboidVAO
{
public:
	CuboidVAO();
	~CuboidVAO();
	void Init();
	void Destroy();
	void Bind();
	void Unbind();
private:
	VertexArrayObject m_VAO;
};

#endif

