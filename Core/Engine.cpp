#include "Engine.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Timer.h"
#include "InputHandler.h"

#include "Components/BaseRenderComponent.h"
#include "Components/Transform.h"
#include "Utils/Logger.h"
#include "Scene/Scene.h"

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

		Logger::GetInstance()->Init("HIPHOP TEST");
		Timer::GetInstance()->Reset();


		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f,0.0f,0.0f,1.0f);

		PrepareScene();

		return true;
	}

	void Engine::PrepareScene()
	{
		m_Scene = new Scene(this,"TestScene");
		m_Scene->Init();

		CreateActor(0,"Floor","Box",glm::vec3(0.0f,0.0f,-2.0f),glm::vec3(50.0f,2.0f,50.0f));
		CreateActor(1, "Ball", "Sphere", glm::vec3(0.0f, 10.0f, -2.0f), glm::vec3(1));
		CreateActor(2, "Sky", "Sky", glm::vec3(0.0f), glm::vec3(1));
	}


	void Engine::CreateActor(int id, const std::string& name,const std::string& renderComponentName,glm::vec3 pos,glm::vec3 scale)
	{
		auto newActor = std::make_shared<GameActor>(name,id);
		auto transformComponent = std::make_shared<Transform>(pos,scale);
		transformComponent->SetOwner(newActor);
		newActor->AddComponent(transformComponent);
		
		if (renderComponentName == "Box")
		{
			auto renderComponent = std::make_shared<BoxRenderComponent>();
			renderComponent->SetOwner(newActor);
			newActor->AddComponent(renderComponent);
			auto sceneNode = renderComponent->CreateBaseNode();
			if(sceneNode)
				m_Scene->AddChild(sceneNode);
		}
		if (renderComponentName == "Sphere")
		{
			auto renderComponent = std::make_shared<SphereRenderComponent>(10,20,20);
			renderComponent->SetOwner(newActor);
			newActor->AddComponent(renderComponent);
			auto sceneNode = renderComponent->CreateBaseNode();
			if (sceneNode)
				m_Scene->AddChild(sceneNode);
		}
		if (renderComponentName == "Sky")
		{
			auto renderComponent = std::make_shared<SkyRenderComponent>();
			renderComponent->SetOwner(newActor);
			newActor->AddComponent(renderComponent);
			auto sceneNode = renderComponent->CreateBaseNode();
			if (sceneNode)
				m_Scene->AddChild(sceneNode);
		}

		m_Actors.push_back(newActor);
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

	void Engine::Tick()
	{
		Timer::GetInstance()->Update();
		InputHandler::GetInstance()->PollKeyActions();		
		double dt = Timer::GetInstance()->GetDeltaTimeInSeconds();

		//Fixed update physics

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_Scene->Render();

		SwapBuffers();
	
	}


	void Engine::OnKeyAction(int key, int action)
	{
		if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
		{
			for (auto actor : m_Actors)
				actor->Destroy();
			
			m_Actors.clear();
			m_Scene->Destroy();

			delete m_Scene;

			CloseWindow();
		//	GLFWGame::Destroy();
		}

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


