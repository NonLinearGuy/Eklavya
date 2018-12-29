#pragma once

#include<memory>
#include<list>
#include "Camera.h"

namespace HipHop
{
	class BaseScene
	{
	public:
		virtual bool Initialize() = 0;
		virtual void Destroy() = 0;
		virtual void Tick(float deltaTime) = 0;
		virtual void Render() = 0;

		virtual void AddChild(std::shared_ptr<BaseScene> pChildScene) {}
		virtual void RemoveChild(std::shared_ptr<BaseScene> pChildScene) {}

		inline const std::string& GetName() const { return m_Name; }
		inline void SetName(const std::string& pSceneName) { m_Name = pSceneName; }

	private:
		std::string m_Name;
		std::list< std::shared_ptr<BaseScene> > m_Childrens;
	};
}

#define OVERRIDE_BASESCENE_METHODS bool Initialize()override;\
								void Destroy()override;\
								void Tick(float deltaTime) override;\
								void Render();
