#include "TestScene.h"
#include "Cuboid.h"
#include "Globals.h"
#include "Player.h"

namespace HipHop
{

	TestScene::TestScene()
	{
	}


	TestScene::~TestScene()
	{
	}

	bool TestScene::Initialize()
	{
	 	m_RootActor = std::make_shared<RootActor>();
		std::shared_ptr<Player> cameraTarget = std::make_shared<Player>();
		std::shared_ptr<Cuboid> floor = std::make_shared<Cuboid>();
		floor->GetTransform().SetScale(glm::vec3(100.0f,5.0f,100.0f));
		floor->GetTransform().SetPosition(glm::vec3(0.0f,-5.0f,-5.0f));
		cameraTarget->GetTransform().SetScale(glm::vec3(10.0f,20.0f,10.0f));
		cameraTarget->GetTransform().SetPosition(glm::vec3(0.0f, 10.0f, 0.0f));
		m_Camera = std::make_shared<Camera>(cameraTarget);

		floor->SetParent(m_RootActor);
		cameraTarget->SetParent(m_RootActor);
		m_RootActor->AddChild(floor);
		m_RootActor->AddChild(cameraTarget);

		m_RootActor->Init();

		
		float aspectRatio = Globals::gEngine->GetCurrentContext()->GetAspectRatio();
		m_Projection = glm::perspective(45.0f,aspectRatio,0.1f,1000.0f);

		return true;
	}

	void TestScene::Destroy()
	{
		m_RootActor->Destroy();
	}

	void TestScene::Tick(float deltaTime)
	{
		m_RootActor->Tick(deltaTime);
	}

	void TestScene::Render()
	{
		m_RootActor->PreRender();
		m_RootActor->Render();
		m_RootActor->PostRender();
	}

	//ROOT ACTOR

	RootActor::RootActor()
		:GameObject()
	{

	}

	RootActor::~RootActor()
	{

	}

	bool RootActor::Init()
	{
		for (auto child : m_Children)
			child->Init();
		return true;
	}

	void RootActor::Destroy()
	{
		for (auto child : m_Children)
			child->Destroy();
	}

	void RootActor::Tick(float deltaTime)
	{
		for (auto child : m_Children)
			child->Tick(deltaTime);
	}

	void RootActor::PreRender()
	{
		for (auto child : m_Children)
			child->PreRender();
	}

	void RootActor::Render()
	{
		for (auto child : m_Children)
			child->Render();
	}

	void RootActor::PostRender()
	{
		for (auto child : m_Children)
			child->PostRender();
	}
}
