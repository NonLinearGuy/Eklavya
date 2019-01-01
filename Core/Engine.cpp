#include "Engine.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Timer.h"
#include "InputHandler.h"

#include "Components/BaseRenderComponent.h"
#include "Components/Transform.h"
#include "Utils/Logger.h"
#include "Scene/Scene.h"
#include"Components/MovementComponent.h"
#include "Renderer/GLRenderer.h"


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

		m_Text = new TextRenderer("gothic");

		m_Renderer = std::make_shared<GLRenderer>(width,height);
		m_Renderer->Initialize(this);
		PrepareScene();

		return true;
	}

	void Engine::PrepareScene()
	{
		m_Scene = new Scene(this,"TestScene");
		m_Scene->Init(m_Renderer);

		CreateActor(0,"Floor","Box",glm::vec3(0.0f,0.0f,-2.0f),glm::vec3(300.0f,3.0f,300.0f));
		CreateActor(1, "wall0", "Box", glm::vec3(-50.0f, 11, 10.0f), glm::vec3(20.0f, 100.0f, 10.0f));
		CreateActor(2, "wall1", "Box", glm::vec3(50.0f, 11.0f, -2.0f), glm::vec3(20.0f, 50.0f, 50.0f));
		CreatePawnActor(3, "Light", "Sphere", glm::vec3(100.0f), glm::vec3(1),10);
		auto newActor = CreateActor(4, "Ball", "Sphere", glm::vec3(15.0f, 20.0f, -2.0f), glm::vec3(1),20);
		CreateActor(5, "Sky", "Sky", glm::vec3(0.0f), glm::vec3(1));

		auto movement = std::make_shared<MovementComponent>();
		movement->SetOwner(newActor);
		newActor->AddComponent(movement);
	}


	std::shared_ptr<GameActor> Engine::CreateActor(int id, const std::string& name,const std::string& renderComponentName,glm::vec3 pos,glm::vec3 scale,float radius)
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
			auto renderComponent = std::make_shared<SphereRenderComponent>(radius,20,20);
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
		return newActor;
	}


	std::shared_ptr<PawnActor> Engine::CreatePawnActor(int id, const std::string& name, const std::string& renderComponentName, glm::vec3 pos, glm::vec3 scale, float radius)
	{
		auto newPawnActor = std::make_shared<PawnActor>(name, id);
		auto transformComponent = std::make_shared<Transform>(pos, scale);
		transformComponent->SetOwner(newPawnActor);
		newPawnActor->AddComponent(transformComponent);

		if (renderComponentName == "Box")
		{
			auto renderComponent = std::make_shared<BoxRenderComponent>();
			renderComponent->SetOwner(newPawnActor);
			newPawnActor->AddComponent(renderComponent);
			auto sceneNode = renderComponent->CreateBaseNode();
			if (sceneNode)
				m_Scene->AddChild(sceneNode);
		}
		if (renderComponentName == "Sphere")
		{
			auto renderComponent = std::make_shared<SphereRenderComponent>(radius, 20, 20);
			renderComponent->SetOwner(newPawnActor);
			newPawnActor->AddComponent(renderComponent);
			auto sceneNode = renderComponent->CreateBaseNode();
			if (sceneNode)
				m_Scene->AddChild(sceneNode);
		}
		if (renderComponentName == "Sky")
		{
			auto renderComponent = std::make_shared<SkyRenderComponent>();
			renderComponent->SetOwner(newPawnActor);
			newPawnActor->AddComponent(renderComponent);
			auto sceneNode = renderComponent->CreateBaseNode();
			if (sceneNode)
				m_Scene->AddChild(sceneNode);
		}

		m_Actors.push_back(newPawnActor);
		return newPawnActor;
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
		
		for (auto actor : m_Actors)
			actor->Tick(dt);
		
		
		m_Renderer->RunAllPasses(m_Scene);
		m_Renderer->RenderOutput();
	/*	m_Renderer->ChangeState(new BlendState());
		m_Text->Text(glm::vec2(50.0f), 20, glm::vec3(0.0f), "FPS : %d", Timer::GetInstance()->GetFramesPerSecond());
		m_Renderer->ChangeState(new DefaultState());*/

		SwapBuffers();

	}


	void Engine::OnKeyAction(int key, int action)
	{
		if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
		{
			CloseWindow();
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


