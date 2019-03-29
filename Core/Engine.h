#pragma once

#include "GLFWGame.h"

#include <map>
#include <string>

#include "Components/GameActor.h"
#include "Components/PawnActor.h"
#include <glm/glm.hpp>
#include "UI/TextRenderer.hpp"
#include "Scene/DebugCamera.h"
#include "Scene/Scene.h"
#include "Physics/Collider.h"


class GLRenderer;
class IEventData;
class DiagManager;
class Physics;

	class Engine final : public GLFWGame
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
		inline GLWindowContext* GetWindowContext() { return m_CurrentContext; }
		
		std::shared_ptr<GameActor> GetActor(ActorID id);
		
		void AddActor(std::shared_ptr<GameActor> newActor) { m_Actors.push_back(newActor); }
		
		void PrepareScene();

		void LoadAssets();
		void ReleaseAssets();

	private:

		std::shared_ptr<GLRenderer> m_Renderer;
		Scene * m_Scene;
		std::vector< std::shared_ptr<GameActor> > m_Actors;
		std::shared_ptr<TextRenderer> m_Text;
		float deltaScale;
		std::shared_ptr<DiagManager> m_Diagnostics;
		std::shared_ptr<Physics> m_Physics;
		std::shared_ptr<RigidBodyComponent> m_RB;
		glm::vec3 m_Force;
	};
