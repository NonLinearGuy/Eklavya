#pragma once

#include "GLFWGame.h"
#include "BaseScene.h"
#include <map>
#include <string>

#include "TestScene.h"
class Renderer;
class PhysicsEngine;

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
	private:

		TestScene * m_CurrentScene;
		//PhysicsEngine* m_PhysicsEngine;
		//Renderer* m_Renderer;
	};
}

