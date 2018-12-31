#pragma once

#include "GLFWGame.h"

#include <map>
#include <string>

#include "Components/GameActor.h"
#include <glm/glm.hpp>

class Scene;
namespace HipHop
{
	class Engine final : public HipHop::GLFWGame
	{
	public:
		Engine();
		~Engine();

		bool Initialize(const std::string& windowName, int width, int height, bool fullScreen)override;
		void Tick() override;
		void Destroy()override;

		void OnKeyAction(int key, int action)override;
		void OnMouseAction(int key, int action)override;
		void OnCursorMoved(double x, double y)override;

		inline const GLWindowContext* GetWindowContext() { return m_CurrentContext; }
	//	inline const PhysicsEngine* GetPhysicsEngine() { return m_PhysicsEngine; }
		//inline const Renderer* GetRenderer() { return m_Renderer; }

		std::shared_ptr<GameActor> GetActor(ActorID id);

		//temps

		
		void CreateActor(int id,const std::string& name,const std::string& renderCompName,glm::vec3 pos,glm::vec3 scale);

		void PrepareScene();
	private:

		Scene * m_Scene;
		std::vector< std::shared_ptr<GameActor> > m_Actors;
	};
}

