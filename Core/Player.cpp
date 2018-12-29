#include "Player.h"
#include "Globals.h"
#include "Timer.h"
using namespace HipHop;


Player::Player()
	:
	GameObject()
{

}

Player::~Player()
{
}


void Player::PollKeyAction()
{
	float dt = Timer::GetInstance()->GetDeltaTimeInSeconds();

	if (IsKeyPressed(GLFW_KEY_W))
		MoveForward(dt);
	if (IsKeyPressed(GLFW_KEY_S))
		MoveBackward(dt);
	if (IsKeyPressed(GLFW_KEY_A))
		TurnLeft(dt);
	if (IsKeyPressed(GLFW_KEY_D))
		TurnRight(dt);
}

void Player::MoveForward(float dt)
{
	glm::mat4 model = GetTransform().GetModelMatrix();
	glm::vec3 oldPosition = glm::vec3(model[3]);
	glm::vec3 newPosition = oldPosition + ((dt * 50.0f) * glm::vec3(0.0f, 0.0f, -1.0f));
	GetTransform().SetPosition(newPosition);
}

void Player::MoveBackward(float dt)
{
	glm::mat4 model = GetTransform().GetModelMatrix();
	glm::vec3 oldPosition = glm::vec3(model[3]);
	glm::vec3 newPosition = oldPosition + ((dt * 50.0f) * glm::vec3(0.0f, 0.0f, 1.0f));
	GetTransform().SetPosition(newPosition);
}

void Player::TurnLeft(float dt)
{
	float yaw = GetTransform().GetRotation().y;
	yaw += dt * 20.0f;
	GetTransform().SetRotation(glm::vec3(0.0f, yaw, 0.0f));
}

void Player::TurnRight(float dt)
{
	float yaw = GetTransform().GetRotation().y;
	yaw -= dt * 20.0f;
	GetTransform().SetRotation(glm::vec3(0.0f, yaw, 0.0f));
}



bool Player::Init()
{

	GameObject::Init();

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	m_VAOConfig.Create(vertices, sizeof(vertices), 0);
	m_VAOConfig.SetPosPtr(3, 0, sizeof(float) * 8);
	m_VAOConfig.SetNormalPtr(sizeof(float) * 3, sizeof(float) * 8);
	m_VAOConfig.SetTexPtr(sizeof(float) * 6, sizeof(float) * 8);


	m_Shader.AddAndCompile("test_vs.glsl", EShaderType::VERTEX);
	m_Shader.AddAndCompile("test_fs.glsl", EShaderType::FRAGMENT);
	m_Shader.Build();

	m_Shader.Use();
	m_Shader.SetInt("floorTexture", 0);

	
	m_FloorTexture.CreateTexture("textures/diffuse.png");

	return true;
}

void Player::Destroy()
{

	GameObject::Destroy();

	m_VAOConfig.Destroy();
}

void Player::Tick(float pDeltaTime)
{
	static float angle = 0;

	GameObject::Tick(pDeltaTime);

}

void Player::PreRender()
{
	m_Shader.Use();
	glm::mat4 model = GetWorldMatrix();
	glm::mat4 view = Globals::gTestScene->GetCamera()->GetView();
	glm::mat4 projection = Globals::gTestScene->GetProjection();

	m_Shader.SetMat4("projection", projection);
	m_Shader.SetMat4("view", view);
	m_Shader.SetMat4("model", model);
}

void Player::Render()
{
	GameObject::Render();

	m_Shader.Use();
	m_FloorTexture.BindToUnit(GL_TEXTURE0);
	m_VAOConfig.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	m_VAOConfig.Unbind();
}


void Player::PostRender()
{

}