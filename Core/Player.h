#pragma once
#include "UserInputListener.h"
#include "GameObject.h"
#include "ShaderProgram.h"
#include "VertexArrayObject.h"
#include "Texture2D.h"

class Player :
	public GameObject,
	public UserInputListener
{
public:
	
	OVERRIDE_GAME_OBJECT_METHODS

		void PollKeyAction()override;
	
	void MoveForward(float dt);
	void MoveBackward(float dt);
	void TurnLeft(float dt);
	void TurnRight(float dt);
	Player();
	~Player();

private:

	HipHop::ShaderProgram m_Shader;
	VertexArrayObject m_VAOConfig;
	HipHop::Texture2D m_FloorTexture;

	float m_Speed;
};

