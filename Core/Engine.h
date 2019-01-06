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

class GLRenderer;


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

		//temps
		std::shared_ptr<GameActor> CreateActor(int id,const std::string& name,const std::string& renderCompName,glm::vec3 pos,glm::vec3 scale,float radius = 1.0f);
		std::shared_ptr<PawnActor> CreatePawnActor(int id, const std::string& name, const std::string& renderCompName, glm::vec3 pos, glm::vec3 scale, float radius = 1.0f);
		std::shared_ptr<BaseNode> GetBaseNode(std::shared_ptr<GameActor> actor);

		void PrepareScene();
	private:

		std::shared_ptr<GLRenderer> m_Renderer;
		Scene * m_Scene;
		std::vector< std::shared_ptr<GameActor> > m_Actors;
		TextRenderer* m_Text;
	};
