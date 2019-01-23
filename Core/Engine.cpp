#include "Engine.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Timer.h"
#include "InputHandler.h"

#include "Utils/Logger.h"
#include "Scene/Scene.h"
#include"Components/MovementComponent.h"
#include "Renderer/GLRenderer.h"
#include "Physics/ProjectileComponent.h"
#include "Physics/CollisionDetector.h"
#include "Physics/RigidBody.h"
#include "Physics/CollisionDetector.h"

#include "ActorFactory.h"
#include "Globals.h"


	Engine::Engine()
	{
		g_Engine = this;
	}

	Engine::~Engine()
	{
	}

	bool Engine::Initialize(const std::string & windowName, int width, int height, bool fullScreen)
	{
		if (!GLFWGame::Initialize(windowName, width, height, fullScreen))
			return false;
		GLFWGame::HideMouse();
		

		Logger::GetInstance()->Init("HIPHOP TEST");
		Timer::GetInstance()->Reset();

		m_Text = new TextRenderer("gothic");

		m_Renderer = std::make_shared<GLRenderer>(m_CurrentContext);
		m_Renderer->Initialize();

		m_Scene = new Scene(this, m_Renderer, "TestScene");
		m_Scene->Init();

		deltaScale = 1.0f;


		return true;
	}

	void Engine::PrepareScene()
	{
		
	}

	void Engine::Tick()
	{
		Timer::GetInstance()->Update();
		InputHandler::GetInstance()->PollKeyActions();
		double dt = Timer::GetInstance()->GetDeltaTimeInSeconds();

		//Physics Engine Update

		//Render 3D

		//Render 2D on top
		
		m_CurrentContext->SwapBuffers();

	}


	std::shared_ptr<GameActor> Engine::GetActor(ActorID id)
	{
		for (auto iter = m_Actors.begin(); iter != m_Actors.end(); ++iter)
		{
			if (id == (*iter)->GetID())
			{
				return *iter;
			}
		}
	}


	void Engine::Destroy()
	{
		m_Scene->Destroy();
	}


	void Engine::OnKeyAction(int key, int action)
	{
		if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
		{
			CloseWindow();
		}
		if (GLFW_KEY_PAGE_UP == key && action == GLFW_PRESS)
			deltaScale += .1;
		if (GLFW_KEY_PAGE_DOWN == key && action == GLFW_PRESS)
			deltaScale -= .1;

		if (key == GLFW_KEY_DELETE && action == GLFW_PRESS)
			m_Scene->RemoveAnObject();

		InputHandler::GetInstance()->OnKeyAction(key, action);
	}

	void Engine::OnMouseAction(int key, int action)
	{
		InputHandler::GetInstance()->OnMouseAction(key, action);
	}

	void Engine::OnCursorMoved(double x, double y)
	{
		InputHandler::GetInstance()->OnCursorMove(x, y);

		
	}

