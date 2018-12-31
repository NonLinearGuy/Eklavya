//Declaration header

#include"Texture2D.h"


#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>

namespace HipHop
{
	Texture2D::Texture2D()
	{

	}

	Texture2D::~Texture2D()
	{

	}

	Texture2D::Texture2D(const std::string& path)
	{
		if (!path.empty())
			CreateTexture(path);
	}

	void Texture2D::Delete()
	{
		glDeleteTextures(1, &mID);
		stbi_image_free(mImage);
	}

	GLuint Texture2D::CreateTexture(const std::string& path, bool repeat) {
		int width, height;
		int channels;
		unsigned char *image = stbi_load(path.c_str(), &width, &height, &channels, 0);
		mImage = image;
		mSize = glm::vec2(width, height);
		mFormat = (channels == 4) ? GL_RGBA : GL_RGB;
		glGenTextures(1, &mID);
		glBindTexture(GL_TEXTURE_2D, mID);
		if (repeat)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, mFormat, width, height, 0, mFormat, GL_UNSIGNED_BYTE, mImage);
		glBindTexture(GL_TEXTURE_2D, 0);

		return mID;
	}


	void Texture2D::BindToUnit(GLenum textureUnit) const
	{
		glActiveTexture(textureUnit);
		glBindTexture(GL_TEXTURE_2D, mID);
	}


	void Texture2D::Unbind()const
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

