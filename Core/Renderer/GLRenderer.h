#pragma once

#include <map>
#include "RenderState.h"

class Scene;
class GLWindowContext;
class GLFWGame;
class ShaderProgram;
class Engine;
class IRenderPass;


enum class EShaderProgram
{
	SOLID,
	UNLIT_SOLID,
	SKYBOX,
	SHADOW,
	OUTLINED,
	WATER_PASS
};


class GLRenderer 
{
public:

	friend class IRenderPass;

	GLRenderer(GLWindowContext* context);
	~GLRenderer();
	bool Initialize();
	void Destroy();
	void RunAllPasses(Scene* scene);
	void RenderOutput();
	void ChangeState(IRenderState* NewRenderState);
	void SetShaderProgram(EShaderProgram program);
	std::shared_ptr<ShaderProgram> GetActiveProgram() { return m_ActiveProgram; }
	void SetWorldMatrix(glm::mat4& worldTransform);
	void SetViewMatrix(glm::mat4& view);
	void SetProjectionMatrix(glm::mat4& projection);
	void SetViewport(int, int, int, int);
	void SetShadowPassValues();
	void SetWaterPassValues(ShaderProgram shader);
	void SetLightValues(glm::vec3 viewSpacePos,class LightSource* light);

	

private:

	
	void SetClearColor(float r, float g, float b, float a);
	void SwapBuffers();

	std::shared_ptr<ShaderProgram> LoadShaderProgram(const std::string& name);

	IRenderState* m_CurrentState;
	GLuint m_VAO;
	GLWindowContext* m_Context;
	std::map<EShaderProgram, std::shared_ptr<ShaderProgram>> m_Programs;
	std::shared_ptr < ShaderProgram > m_ActiveProgram;
	std::shared_ptr	< ShaderProgram > m_MainOutputShader;
	std::vector<IRenderPass*> m_RenderPasses;
};
