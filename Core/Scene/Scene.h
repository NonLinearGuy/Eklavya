#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <vector>
#include <stack>

namespace HipHop
{
	class Engine;
}


class DebugCamera;
class BaseNode;
class GLRenderer;

class Scene
{
public:

	Scene(HipHop::Engine* engineRef,std::shared_ptr<GLRenderer> renderer,const std::string& pName);
	~Scene();

	void Init();
	void Destroy();
	void Tick(float deltaTime);
	void AddChild(std::shared_ptr<BaseNode> pNewNode);
	void RemoveChild(std::shared_ptr<BaseNode> pNode);
	void Render();


	inline std::shared_ptr<DebugCamera> GetCamera()
	{
		return m_Camera;
	}

	inline HipHop::Engine* GetEngineRef() { return m_EngineRef; }

	glm::mat4 GetProjection() { return m_Projection; }

	inline std::shared_ptr<GLRenderer> GetRenderer()
	{
		return m_Renderer;
	}
	
	void SetRenderer(std::shared_ptr<GLRenderer> renderer)
	{
		m_Renderer = renderer;
	}

	void PushMatrix(const glm::mat4& model);

	void PopMatrix()
	{
		m_MatrixStack.pop();
	}

private:

	std::shared_ptr<DebugCamera> m_Camera;
	glm::mat4 m_Projection;
	std::string m_Name;
	HipHop::Engine* m_EngineRef;
	std::vector<std::shared_ptr<BaseNode>> m_Groups;
	std::shared_ptr<GLRenderer> m_Renderer;
	std::stack<glm::mat4> m_MatrixStack;
	
};

