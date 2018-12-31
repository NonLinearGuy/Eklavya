#include "Cubemap.h"
#include <stb_image.h>

Cubemap::Cubemap()
{
}

Cubemap::~Cubemap()
{
}

void Cubemap::Create(const std::vector<std::string>& orderedFaceNames)
{
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

	int width, height, channels;
	unsigned char* image = nullptr;

	for (int index = 0; index < 6; ++index)
	{
		image = stbi_load(("Assets/Skybox/" + orderedFaceNames[index] + ".tga").c_str(), &width, &height, &channels, 0);
		GLenum format = channels == 4 ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
		stbi_image_free(image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Cubemap::BindToUnit(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP,m_ID);
}

void Cubemap::Delete()
{
	glDeleteTextures(GL_TEXTURE_CUBE_MAP,&m_ID);
}
