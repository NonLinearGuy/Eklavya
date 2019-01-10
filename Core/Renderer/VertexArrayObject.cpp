#include "VertexArrayObject.h"
#include <vector>

const float PI = 3.1415926535;

VertexArrayObject::VertexArrayObject()
{	
}

VertexArrayObject::~VertexArrayObject()
{
}

void VertexArrayObject::Destroy()
{
	glDeleteBuffers(1,&m_VBO);
	glDeleteBuffers(1, &m_EBO);
	glDeleteVertexArrays(1,&m_VAO);
}

void VertexArrayObject::SetPosPtr(int size,GLsizei startOffset,GLsizei strideOffset)
{
	Bind();
	glBindBuffer(GL_ARRAY_BUFFER,m_VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,size,GL_FLOAT,GL_FALSE,strideOffset,(void*)startOffset);
	Unbind();
}

void VertexArrayObject::SetNormalPtr(GLsizei startOffset, GLsizei strideOffset)
{
	Bind();
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, strideOffset, (void*)startOffset);
	Unbind();
}

void VertexArrayObject::SetTexPtr(GLsizei startOffset, GLsizei strideOffset)
{
	Bind();
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, strideOffset, (void*)startOffset);
	Unbind();
}

SphereVAO::SphereVAO()
{
	
}

SphereVAO::~SphereVAO()
{
}

void SphereVAO::Init(float radius, int stackCount, int sectorCount)
{
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoords;

	float x = 0, y = 0, z = 0, xy = 0;                              // vertex position
	float nx = 0, ny = 0, nz = 0, lengthInv = 1.0f / radius;    // vertex normal
	float s = 0, t = 0;                                     // vertex texCoord

	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle = 0.0f, stackAngle = 0.0f;

	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);

			// normalized vertex normal (nx, ny, nz)
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			normals.push_back(nx);
			normals.push_back(ny);
			normals.push_back(nz);

			// vertex tex coord (s, t) range between [0, 1]
			s = (float)j / sectorCount;
			t = (float)i / stackCount;
			texCoords.push_back(s);
			texCoords.push_back(t);
		}
	}

	// generate CCW index list of sphere triangles
	std::vector<int> indices;
	int k1, k2;
	for (int i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);     // beginning of current stack
		k2 = k1 + sectorCount + 1;      // beginning of next stack

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (stackCount - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}

	std::vector<float> buffer;
	int bufferSize = sizeof(float) * (vertices.size() + normals.size() + texCoords.size());
	buffer.reserve(bufferSize);
	buffer.insert(buffer.end(), vertices.begin(), vertices.end());
	buffer.insert(buffer.end(), normals.begin(), normals.end());
	buffer.insert(buffer.end(), texCoords.begin(), texCoords.end());

	m_VAO.Create(buffer.data(), bufferSize, indices.data(), sizeof(int) * indices.size());
	m_VAO.SetPosPtr(3, 0, 0);
	m_VAO.SetNormalPtr(sizeof(float) * vertices.size(), 0);
	m_VAO.SetTexPtr(sizeof(float) * (vertices.size() + normals.size()), 0);

	m_IndicesSize = indices.size();
}

void SphereVAO::Destroy()
{
	m_VAO.Destroy();
}

void SphereVAO::Bind()
{
	m_VAO.Bind();
}

void SphereVAO::Unbind()
{
	m_VAO.Unbind();
}
