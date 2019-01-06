//
// Created by Ankit Singh on 18-11-2018.
//

#include "TextRenderer.hpp"
#include <cstdio>


	TextRenderer::TextRenderer()
	{

	}

	TextRenderer::TextRenderer(const std::string& fontName)
	{
		Init(fontName);
	}

	TextRenderer::~TextRenderer()
	{
	}

	void TextRenderer::Init(const std::string& fontName)
	{
		mTextureAtlas.CreateTexture("Assets/Fonts/" + fontName + ".png");

		FILE *file = nullptr;
		file = fopen(("Assets/Fonts/" + fontName + ".fnt").c_str(), "r");

		while (true)
		{
			character c;
			int id;
			char result = fscanf(file, "%d,%d,%d,%d,%d,%d,%d,%d,",
				&id,
				&c.position.x,
				&c.position.y,
				&c.size.x,
				&c.size.y,
				&c.offsets.x,
				&c.offsets.y,
				&c.advanceX
			);

			mCharacters.insert(std::pair<char, character>(static_cast<char>(id), c));

			if (result == EOF) break;
		}

		fclose(file);

		m_Shader.AddAndCompile("assets/shaders/text_vs.glsl", EShaderType::VERTEX);
		m_Shader.AddAndCompile("assets/shaders/text_fs.glsl", EShaderType::FRAGMENT);
		m_Shader.Build();

		glm::mat4 projection = glm::ortho(0.0f,1024.0f,640.0f,0.0f,0.0f,1.0f);
		m_Shader.Use();
		m_Shader.SetMat4("projection",projection);

		//Setting up quad
		float points[] =
		{
				0.0f,0.0f,
				1.0f,0.0f,
				0.0f,1.0f,
				1.0f,1.0f
		};

		glGenVertexArrays(1, &mVAO);
		glGenBuffers(1, &mVBO);

		glBindVertexArray(mVAO);

		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

		//Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));

		glBindVertexArray(0);

	}


	void TextRenderer::Text(
		const glm::vec2& pPosition,
		float pSize,
		const glm::vec3& pColor,
		const char* pFormat,
		...
	)
	{

		float width = MapToRange(1.0f, 300.0f, .43f, .6f, pSize);
		float edge = 0.2f - MapToRange(1.0f, 300.0f, 0.1f, 0.2f, pSize);
		pSize = MapToRange(1.0f, 300.0f, 0.2f, 5.0f, pSize);

		glm::vec2 tempPos = pPosition;

		//Initializing variable argument list
		va_list lst;
		va_start(lst, pFormat);
		std::string text;

		//Extracting data from variable argument list
		for (const char* traverse = pFormat; *traverse != '\0'; traverse++)
		{
			while (true)
			{
				if (*traverse == '%' || *traverse == '\0') break;

				text += *traverse;
				traverse++;
			}

			if (*traverse == '\0')
				break;
			traverse++;

			switch (*traverse)
			{
			case 'd':
				text += std::to_string(va_arg(lst, int));
				break;
			case 'f':
				text += std::to_string(va_arg(lst, double));
				break;
			}

		}

		va_end(lst);

		int primCounter = 0; //counts the number of text


		//Iterating through the string and calculating attributes of each character
		for (auto iter = text.begin(); iter != text.end(); iter++)
		{

			character c = mCharacters[*iter];


			if (*iter == '\n')
			{
				tempPos.y += pSize * 70.0f;
				tempPos.x = pPosition.x;
				continue;
			}



			float dx = float(c.position.x) / mTextureAtlas.GetSize().x;
			float dy = float(c.position.y) / mTextureAtlas.GetSize().y;
			float dw = float(c.size.x) / mTextureAtlas.GetSize().x;
			float dh = float(c.size.y) / mTextureAtlas.GetSize().y;


			mTexCoords.push_back(glm::vec4(dx, dy, dw, dh));


			mOffsets.push_back(tempPos + (glm::vec2(c.offsets)*pSize));
			mScalers.push_back(glm::vec2(c.size)*pSize);

			tempPos.x += (c.advanceX - 8)*pSize;

			primCounter++;

		}


		//Generating some needed buffers for offsets,scales and texture coordinates

		glGenBuffers(1, &mVBOTexCoords);
		glGenBuffers(1, &mVBOOffsets);
		glGenBuffers(1, &mVBOScalers);



		glBindVertexArray(mVAO);


		//TEXTURE COORDS
		glBindBuffer(GL_ARRAY_BUFFER, mVBOTexCoords);
		glBufferData(GL_ARRAY_BUFFER,
			sizeof(glm::vec4)*mTexCoords.size(),
			mTexCoords.data(),
			GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glVertexAttribDivisor(1, 1);

		//OFFSETS
		glBindBuffer(GL_ARRAY_BUFFER, mVBOOffsets);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*mOffsets.size(), mOffsets.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glVertexAttribDivisor(2, 1);

		//SCALERS
		glBindBuffer(GL_ARRAY_BUFFER, mVBOScalers);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*mScalers.size(), mScalers.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribDivisor(3, 1);


		glBindVertexArray(0);

		m_Shader.Use();

		m_Shader.SetVec3("font_color", pColor);
		m_Shader.SetFloat("width", width);
		m_Shader.SetFloat("edge", edge);
		m_Shader.SetInt("use_color", 1);

		//Binding the font texture at GL_TEXTURE0
		m_Shader.SetInt("texture_atlas", 0);
		mTextureAtlas.BindToUnit(GL_TEXTURE0);
		glBindVertexArray(mVAO);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, primCounter);
		glBindVertexArray(0);


		glDeleteBuffers(1, &mVBOOffsets);
		glDeleteBuffers(1, &mVBOScalers);
		glDeleteBuffers(1, &mVBOTexCoords);

		mOffsets.clear();
		mScalers.clear();
		mTexCoords.clear();

	}