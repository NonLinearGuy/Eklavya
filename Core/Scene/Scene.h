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
class IEventData;
class ContactsNode;


/*
Draw all opaque objects first.
Sort all the transparent objects.
Draw all the transparent objects in sorted order.
*/


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

	//Listeners
	void OnActorCreated(std::shared_ptr<IEventData> data);
	void OnActorDestroyed(std::shared_ptr<IEventData> data);


	inline std::shared_ptr<DebugCamera> GetCamera() { return m_Camera; }

	inline Engine* GetEngineRef() { return m_EngineRef; }
	
	inline std::shared_ptr<GLRenderer> GetRenderer() { return m_Renderer; }
	
	void SetRenderer(std::shared_ptr<GLRenderer> renderer) { m_Renderer = renderer; }
	
	inline bool ShouldSetMaterialProps() { return m_ShouldSetMaterial; }
	
	
	void PushMatrix(const glm::mat4& model);
	void PopMatrix() { m_MatrixStack.pop(); }

	inline LightSource& GetLightSource() { return m_LightSource; }

private:
	/* 
	This calls decides which groups will 
	be renderered for particular render pass
	and with what configuration
	*/

	/* 
	will render solids which should show shadows,
	avoid : light calculations
	avoid : Frustum culling
	avoid : Setting shader values
	*/
	void ShadowPassRender();

	/* 
	will render all groups 
	- will do frustum culling
	- wil do lighting calculation
	- will set values in shaders for different groups
	*/
	void MainPassRender();

	/* 
	will render solids and skybox with materials
	- lighting calculation will be avoided
	- fructum culling will be avoided
	*/
	void WaterPassRender();
private:
	std::string m_Name;

	std::shared_ptr<DebugCamera> m_Camera;

	Engine* m_EngineRef;

	std::vector<std::shared_ptr<BaseNode>> m_Groups;
	
	std::shared_ptr<GLRenderer> m_Renderer;
	
	std::stack<glm::mat4> m_MatrixStack;
	
	bool m_ShouldSetMaterial;
	
	LightSource m_LightSource;
	
	std::shared_ptr<class WaterNode> m_WaterNode;
	
	std::vector< std::shared_ptr<BaseNode> > m_Nodes;
	
	std::shared_ptr<ContactsNode> m_ContactsNode;
};

