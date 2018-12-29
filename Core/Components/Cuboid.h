#pragma once

#include "VertexArrayObject.h"
#include "GameObject.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "UserInputListener.h"

class Cuboid : public GameObject, public UserInputListener
{
public:
	Cuboid();
	~Cuboid();

	OVERRIDE_GAME_OBJECT_METHODS

		bool m_Rotate;
	bool m_SwitchTexture;

private:
	
	HipHop::ShaderProgram m_Shader;
	VertexArrayObject m_VAOConfig;
	HipHop::Texture2D m_FloorTexture;
};

