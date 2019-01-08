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
		

		Logger::GetInstance()->Init("HIPHOP TEST");
		Timer::GetInstance()->Reset();

		m_Text = new TextRenderer("gothic");

		m_Renderer = std::make_shared<GLRenderer>(m_CurrentContext);
		m_Renderer->Initialize();

		PrepareScene();

		return true;
	}

	void Engine::PrepareScene()
	{
		m_Scene = new Scene(this,m_Renderer,"TestScene");
		m_Scene->Init();

		auto floor = CreateActor(0,"Floor","Box",glm::vec3(0.0f,0.0f,0.0f),glm::vec3(300.0f,10.0f,300.0f));
		m_Scene->AddChild(GetBaseNode(floor));
		auto ground = CreateActor(1, "ground", "Box", glm::vec3(0.0f, -40.0f, 0.0f), glm::vec3(1000.0f, 2.0f, 1000.0f));
		m_Scene->AddChild(GetBaseNode(ground));

		float sphereDiameter = 4;
		int id = 4;
		
		for (float angle = 0; angle <= 360; angle += (sphereDiameter + 4), id++)
		{
			float y = 20.0f;
			float x = 40.0f * cos(toPIE * angle);
			float z = 40.0f * sin(toPIE * angle);

			auto newActor = CreateActor(id, "sphere", "Sphere", glm::vec3(x, y, z), glm::vec3(1), sphereDiameter / 2.0f);
			auto rotate = std::make_shared<RotateXZComponent>();
			rotate->SetOwner(newActor);
			rotate->SetStartAngle(angle);
			newActor->AddComponent(rotate);
			m_Scene->AddChild(GetBaseNode(newActor));
		}

		id = 10000;
		sphereDiameter = 10;
		for (float angle = 0; angle <= 360; angle += (sphereDiameter + 4), id++)
		{
			float x = 0.0f;
			float y = 20 + 60.0f * sin(toPIE * angle);
			float z = 60.0f * cos(toPIE * angle);

			auto newActor = CreateActor(id, "sphere", "Sphere", glm::vec3(x, y, z), glm::vec3(1), sphereDiameter / 2.0f);
			auto rotate = std::make_shared<RotateYZComponent>();
			rotate->SetOwner(newActor);
			rotate->SetStartAngle(angle);
			newActor->AddComponent(rotate);
			m_Scene->AddChild(GetBaseNode(newActor));
		}

		auto sky = CreateActor(100, "Sky", "Sky", glm::vec3(0.0f), glm::vec3(1));
		m_Scene->AddChild(GetBaseNode(sky));

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
		}
		if (renderComponentName == "Sphere")
		{
			auto renderComponent = std::make_shared<SphereRenderComponent>(radius,30,30);
			renderComponent->SetOwner(newActor);
			newActor->AddComponent(renderComponent);
		}
		if (renderComponentName == "Sky")
		{
			auto renderComponent = std::make_shared<SkyRenderComponent>();
			renderComponent->SetOwner(newActor);
			newActor->AddComponent(renderComponent);
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
		}
		if (renderComponentName == "Sphere")
		{
			auto renderComponent = std::make_shared<SphereRenderComponent>(radius, 20, 20);
			renderComponent->SetOwner(newPawnActor);
			newPawnActor->AddComponent(renderComponent);
		}
		if (renderComponentName == "Sky")
		{
			auto renderComponent = std::make_shared<SkyRenderComponent>();
			renderComponent->SetOwner(newPawnActor);
			newPawnActor->AddComponent(renderComponent);
		}

		m_Actors.push_back(newPawnActor);
		return newPawnActor;
	}

	std::shared_ptr<BaseNode> Engine::GetBaseNode(std::shared_ptr<GameActor> actor)
	{
		auto renderComponent = MakeSharedPtr(actor->GetComponent<BaseRenderComponent>(BaseRenderComponent::s_ID));
		return renderComponent->CreateBaseNode();
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
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
		m_Renderer->SetViewport(0.0f,0.0f,m_CurrentContext->GetWidth(),m_CurrentContext->GetHeight());

		m_Scene->Tick(dt);
		m_Scene->Render(ERenderPass::MAIN_PASS);

		//Render UI

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		m_Text->Text(glm::vec2(30.0f),5.0f,glm::vec3(1.0f),"Objects in the scene %d\nObjects Culled %d",m_Scene->GetTotalCount(),m_Scene->GetCulledCount());
		glDisable(GL_BLEND);
		
		m_CurrentContext->SwapBuffers();
	}


	void Engine::OnKeyAction(int key, int action)
	{
		if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
		{
			CloseWindow();
		}

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

