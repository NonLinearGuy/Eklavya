#pragma once

#include <vector>
#include <string>
#include <glad/glad.h>
class Cubemap
{
public:
	Cubemap();
	~Cubemap();
	
	void Create(const std::vector<std::string>& ordererdFaceNames);
	void BindToUnit(GLenum textureUnit);
	void Delete();

private:
	GLuint m_ID;
};