#include "VertexArrayObject.h"



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
