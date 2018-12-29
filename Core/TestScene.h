#pragma once
#include "BaseScene.h"
#include "SceneNode.h"
#include <memory>
#include <vector>
#include "Camera.h"
#include <glm/glm.hpp>


namespace HipHop
{
	class RootActor;

	class TestScene : public BaseScene
	{
	public:
		TestScene();
		~TestScene();

		OVERRIDE_BASESCENE_METHODS
		
		inline std::shared_ptr<Camera> GetCamera()
		{
			return m_Camera;
		}

		inline std::shared_ptr<RootActor> GetRootActor()
		{
			return m_RootActor;
		}

		inline glm::mat4 GetProjection() { return m_Projection; }

	private:
		std::shared_ptr<RootActor> m_RootActor;
		std::shared_ptr<Camera> m_Camera;
		glm::mat4 m_Projection;
	};

	class RootActor : public GameObject
	{
	public:
		RootActor();
		~RootActor();
		OVERRIDE_GAME_OBJECT_METHODS
	private:
	};
}

