#include "Engine.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Timer.h"
#include "InputHandler.h"

#include "Utils/Logger.h"
#include "Scene/Scene.h"
#include "Renderer/GLRenderer.h"
#include "Physics/ProjectileComponent.h"
#include "Physics/CollisionDetector.h"

#include "ActorFactory.h"
#include "Globals.h"
#include "Event/EventDispatcher.h"
#include "Debugging/Diagnostics.h"
#include "Physics/Physics.h"
#include "Random.h"
#include "Components/RigidBodyComponent.h"


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

		m_Text = std::make_shared<TextRenderer>("diag_font");
		m_Diagnostics = std::make_shared<DiagManager>(m_Text);

		m_Renderer = std::make_shared<GLRenderer>(m_CurrentContext);
		m_Renderer->Initialize();

		m_Physics = std::make_shared<Physics>();

		m_Scene = new Scene(this, m_Renderer, "TestScene");
		m_Scene->Init();

		PrepareScene();

		deltaScale = 1.0f;


		return true;
	}

	void Engine::PrepareScene()
	{
		ActorFactory::CreateSky();
		ActorFactory::CreateBoxCollider(glm::vec3(.0f, 10.0f, 0.0f), glm::vec3(2000.0f,100.0f,2000.0f),glm::vec3(0.0f),glm::vec3(0.0f,0.0f,0.0f),0.0f,true,true);
		ActorFactory::CreateModelActor(glm::vec3(0.0f,1000.0f,0.0f),glm::vec3(.5f),glm::vec3(0.0,0.0f,0.0f));
		//ActorFactory::CreateModelActor(glm::vec3(0.0f, 100.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.0, 0.0f, 0.0f));
		//ActorFactory::CreateBoxCollider(glm::vec3(.0f, 20.0f, 240.0f), glm::vec3(500.0f, 300.0f, 10.0f), glm::vec3(0.0f));
	
	}

	void Engine::Tick()
	{
		Timer::GetInstance()->Update();
		InputHandler::GetInstance()->PollKeyActions();
		double dt = Timer::GetInstance()->GetDeltaTimeInSeconds() * deltaScale;
		int fps = Timer::GetInstance()->GetFramesPerSecond();

		DiagManager::sGeneralDiagsMap[EMapKeys::KEY_FPS] = std::to_string(fps);
		DiagManager::sGeneralDiagsMap[EMapKeys::KEY_DELTA] = std::to_string(dt);

		for (auto actor : m_Actors)
			actor->Tick(dt);

		m_Physics->Simulate(dt);
		m_Scene->Tick(dt);
			
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	   	m_Renderer->RunAllPasses(m_Scene);
		m_Scene->Render(ERenderPass::MAIN_PASS);

		m_Renderer->ChangeState(new UIState());
		m_Diagnostics->PostCurrent();
		
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
		if (GLFW_PRESS == action)
		{
			if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
			{
				CloseWindow();
			}
			if (GLFW_KEY_KP_SUBTRACT == key )
				deltaScale -= .1;
			if (GLFW_KEY_KP_ADD == key )
				deltaScale += .1;

			if (key == GLFW_KEY_DELETE )
			{
				auto actor = m_Actors.back();
				auto data = std::make_shared<EventActorCreated>();
				data->m_Actor = actor;

				EventDispatcher::GetInstance().TriggerEvent(EEventType::ACTOR_DESTROYED, data);
			}

			if (key == GLFW_KEY_T)
			{
				static int heightOffset = 0;
	
				glm::vec3 center = Random::GetInstance()->GetOnXZ(-250.0f, 250.0f);
				center.y = 20.0f;
				float radius = 50.0f; Random::GetInstance()->Real(50.0f, 80.0f);
				glm::vec3 direction(1.0f,0.0f,0.0f);
				float distance = 100.0f;

		
				//if(rand() % 2 == 1)
					//ActorFactory::CreateSphereCollider(glm::vec3(0.0f, 300.0f, 0.0f),radius * .5f,glm::vec3(0.0f));
				//else
					//ActorFactory::CreateBoxCollider(glm::vec3(0.0f,300.0f,0.0f), glm::vec3(radius), glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f),true);
	
				//ActorFactory::CreateBoxCollider(glm::vec3(0.0f,1000.0f,0.0f),glm::vec3(200.0f),glm::vec3(0.0f,1.0f,1.0f),glm::vec3(0.0,-1.0f,0.0f),100.0f,false,false);
				//ActorFactory::CreateBoxCollider(glm::vec3(0.0f, 1000.0f, 0.0f), glm::vec3(200.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0, -1.0f, 0.0f), 100.0f, false, false);

			}

			glm::vec3 force = glm::vec3(0.0f);
			glm::vec3 torque = glm::vec3(0.0f);
			if (GLFW_KEY_J == key)
			{
				force.x = -10000.0f;
				torque.z = 1000.0f;
				if (m_RB)
				{
					m_RB->AddForce(force);
					m_RB->AddTorque(torque);
				}
			}
			else if (GLFW_KEY_L == key)
			{
				force.x = 10000.0f;
				torque.z = -1000.0f;
				if (m_RB)
				{
					m_RB->AddForce(force);
					m_RB->AddTorque(torque);
				}
			}
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

