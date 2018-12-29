#include "Engine.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Timer.h"
#include "InputHandler.h"

using namespace HipHop;

namespace HipHop
{
	Engine::Engine()
	{
	}

	Engine::~Engine()
	{
	}

	bool Engine::Initialize(const std::string & windowName, int width, int height, bool fullScreen)
	{
		if (!GLFWGame::Initialize(windowName, width, height, fullScreen))
			return false;
		GLFWGame::HideMouse();
		

		//INITIALIZE : TIMER, LOGGER , PHYSICS ENGINE , RENDERER, CURRENT_SCENE


		return true;
	}

	void Engine::Destroy()
	{
		m_CurrentScene->Destroy();
	}

	void Engine::Tick()
	{
		Timer::GetInstance()->Update();
		InputHandler::GetInstance()->PollKeyActions();		
		double dt = Timer::GetInstance()->GetDeltaTimeInSeconds();

		//Fixed update physics

		//RENDER CURRENT SCENE
	
	}


	void Engine::OnKeyAction(int key, int action)
	{
		if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
			CloseWindow();

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
}


