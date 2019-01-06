#ifndef INC_SHADY_ENGINE_TEXTURE2D_H
#define INC_SHADY_ENGINE_TEXTURE2D_H

#include<string>
#include<glm/glm.hpp>
#include<glad/glad.h>

	class Texture2D
	{
	public:
		//constructor and destructor
		Texture2D();

		Texture2D(const std::string& path);
		GLuint CreateTexture(const std::string& path, bool repeat = false);
		~Texture2D();
		//Getters
		inline GLuint GetID() const
		{
			return mID;
		}
		inline glm::vec2 GetSize() const
		{
			return mSize;
		}
		inline GLenum GetFormat() const
		{
			return mFormat;
		}
		inline unsigned char* GetPixels() const
		{
			return mImage;
		}

		void Delete();
		//functions
		GLvoid BindToUnit(GLenum textureUnit)const;
		GLvoid Unbind()const;
	protected:
		GLuint mID;
		glm::vec2 mSize;
		GLenum mFormat;
		unsigned char* mImage;

	};

#endif