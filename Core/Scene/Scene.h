#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <vector>
#include <stack>
#include "../Renderer/RenderPass.h"

class Engine;
class DebugCamera;
class BaseNode;
class GLRenderer;
class FrustumNode;

struct LightSource
{
	glm::vec3 m_Position;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
	glm::vec3 m_Ambient;
};

class Scene
{
public:

	Scene(Engine* engineRef,std::shared_ptr<GLRenderer> renderer,const std::string& pName);
	~Scene();

	void Init();
	void Destroy();
	void Tick(float deltaTime);
	void AddChild(std::shared_ptr<BaseNode> pNewNode);
	void RemoveChild(std::shared_ptr<BaseNode> pNode);
	void Render(ERenderPass renderPass);


	inline std::shared_ptr<DebugCamera> GetCamera() { return m_Camera; }
	
	inline Engine* GetEngineRef() { return m_EngineRef; }
	
	inline std::shared_ptr<GLRenderer> GetRenderer() { return m_Renderer; }
	
	void SetRenderer(std::shared_ptr<GLRenderer> renderer) { m_Renderer = renderer; }
	
	inline bool ShouldSetMaterialProps() { return m_ShouldSetMaterial; }
	
	inline glm::mat4 GetProjection() { return m_Projection; }
	
	void PushMatrix(const glm::mat4& model);
	void PopMatrix() { m_MatrixStack.pop(); }

	inline LightSource& GetLightSource() { return m_LightSource; }

	inline std::shared_ptr<FrustumNode> GetFrustum() { return m_Frustum; }

private:
	void ShadowPassRender();
	void MainPassRender();
	void WaterPassRender();
private:

	std::shared_ptr<DebugCamera> m_Camera;
	glm::mat4 m_Projection;
	std::string m_Name;
	Engine* m_EngineRef;
	std::vector<std::shared_ptr<BaseNode>> m_Groups;
	std::shared_ptr<GLRenderer> m_Renderer;
	std::stack<glm::mat4> m_MatrixStack;
	bool m_ShouldSetMaterial;
	LightSource m_LightSource;
	std::shared_ptr<FrustumNode>  m_Frustum;
	std::shared_ptr<class WaterNode> m_WaterNode;
	
};

