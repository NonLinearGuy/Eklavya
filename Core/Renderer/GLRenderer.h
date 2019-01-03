#pragma once

#include <vector>

#include "Framebuffer.h"
#include "RenderState.h"
#include "../Engine.h"

class IFramebuffer;
class Scene;
class IRenderPass;


namespace HipHop
{
	class GLWindowContext;
	class GLFWGame;
	class ShaderProgram;
	class Engine;
}


enum class EShaderProgram
{
	SOLID,
	SKYBOX
};

enum class ERenderPass
{
	MIN = 0,
	SHADOW_PASS = MIN,
	MAX
};

class GLRenderer
{
public:
	GLRenderer(HipHop::GLWindowContext* context);
	~GLRenderer();
	bool Initialize();
	void Destroy();
	void RenderWorld(Scene* scene);
	void ChangeState(IRenderState* NewRenderState);
	void SetShaderProgram(EShaderProgram program);
	std::shared_ptr<HipHop::ShaderProgram> GetActiveProgram() { return m_ActiveProgram; }
	void SetWorldMatrix(glm::mat4& worldTransform);
	void SetViewMatrix(glm::mat4& view);
	void SetProjectionMatrix(glm::mat4& projection);

protected:

	void SetViewport(int, int, int, int);
	void SetClearColor(float r, float g, float b, float a);
	void SwapBuffers();

	std::shared_ptr<HipHop::ShaderProgram> LoadShaderProgram(const std::string& name);

	IRenderState* m_CurrentState;
	GLuint m_VAO;
	HipHop::GLWindowContext* m_Context;
	std::map<EShaderProgram, std::shared_ptr<HipHop::ShaderProgram>> m_Programs;
	std::shared_ptr < HipHop::ShaderProgram > m_ActiveProgram;
};

/*class IRenderPass
{
public:
	IRenderPass(ERenderPass type,GLRenderer* renderer);
	virtual ~IRenderPass();
	virtual bool Init() = 0;
	virtual void Destroy() = 0;
	virtual void PreRun() = 0;
	virtual void Run(Scene* scene) = 0;
	virtual void PostRun() = 0;

	IFramebuffer* GetFramebuffer()
	{
		return m_Framebuffer;
	}

	ERenderPass GetType() { return m_Type; }

protected:
	GLRenderer* m_Renderer;
	IFramebuffer* m_Framebuffer;
	ERenderPass m_Type;
};

class ShadowMapPass : public IRenderPass
{
public:

	ShadowMapPass(GLRenderer* renderer);
	~ShadowMapPass();

	bool Init()override;
	void Destroy()override;
	void PreRun()override;
	void Run(Scene* scene)override;
	void PostRun()override;

	GLuint GetDepthAttachment() { return (static_cast<ShadowPassFB*>(m_Framebuffer))->GetDepthAttachment(); }
	HipHop::ShaderProgram& GetShader() { return m_ShadowShader; }

private:

	HipHop::ShaderProgram m_ShadowShader;
};
*/



