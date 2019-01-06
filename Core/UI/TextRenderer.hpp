//
// Created by Ankit Singh on 18-11-2018.
//

#ifndef BURNOUTENGINE_TEXTRENDERER_HPP
#define BURNOUTENGINE_TEXTRENDERER_HPP

//C++ headers
#include<iostream>
#include<map>
#include<string>
#include<cstdarg>
#include<memory>
#include<vector>

//Third party headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"


	class TextRenderer
	{
	
	public:
		//Constructors
		TextRenderer();
		TextRenderer(const std::string& fontName);
		//Destructor
		~TextRenderer();
		/*
			initializes essential data for rendering text
		*/
		void Init(const std::string& fontName);

		/*
			Renders text with a color
		*/
		void Text(const glm::vec2& pPosition,
			float pSize,
			const glm::vec3& pColor,
			const char* pFormat,
			...);

	private:
		struct character
		{
			glm::ivec2 position;
			glm::ivec2 size;
			glm::ivec2 offsets;
			int advanceX;
		};

		const int MAX_BUFFER_SIZE = 1024 * 4;
		std::map<char, character> mCharacters;
		Texture2D mTextureAtlas;
		ShaderProgram m_Shader;
		GLuint mVAO;
		GLuint mVBO;


		//data needed for instancing 
		std::vector<glm::vec2> mOffsets;
		std::vector<glm::vec2> mScalers;
		std::vector<glm::vec4> mTexCoords;

		GLuint mVBOOffsets;
		GLuint mVBOTexCoords;
		GLuint mVBOScalers;

		//Maps values from one range to another
		inline float MapToRange(
			float srcMin,
			float srcMax,
			float destMin,
			float destMax,
			float input
		)
		{
			float slope = (destMax - destMin) / (srcMax - srcMin);
			return (destMin + (slope*input));
		}

	};

#endif 
