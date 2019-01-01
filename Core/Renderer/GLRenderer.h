#pragma once


#include <vector>

#include "Framebuffer.h"
#include "RenderState.h"
#include "ShaderProgram.h"
#include "../Engine.h"

class IFramebuffer;
class Scene;
class IRenderPass;


namespace HipHop
{
	class ShaderProgram;
	class Engine;
}


enum class ERenderPass
{
	MIN = 0,
	SHADOW_PASS = MIN,
	FINAL_OUTPUT_PASS,
	MAX
};

class GLRenderer
{
public:
	GLRenderer(int width, int height);
	~GLRenderer();
	bool Initialize(HipHop::Engine* pEngineRef);
	void Destroy();
	IRenderPass* GetRenderPass(ERenderPass type);

	void RunAllPasses(Scene* scene);
	void RenderOutput();
	void SetViewport(int, int, int, int);
	void ClearBuffers();
	void SetClearColor(float r, float g, float b, float a);
	void ChangeState(IRenderState* NewRenderState);
	void SwapBuffers();
	inline int GetWidth() { return m_Width; }
	inline int GetHeight() { return m_Height; }
	inline ERenderPass GetCurrentRenderPass() { return m_CurrentRenderPass; }
	inline glm::vec3 GetCameraPosition() { return m_EngineRef->GetCameraPosition(); }
protected:
	IRenderState* m_CurrentState;
	std::vector<IRenderPass*> m_RenderPasses;
	int m_Width;
	int m_Height;
	HipHop::ShaderProgram m_MainOutputShader;
	GLuint m_VAO;
	HipHop::Engine* m_EngineRef;
	ERenderPass m_CurrentRenderPass;
};

class IRenderPass
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

class FinalOutputPass : public IRenderPass
{
public:
	
	FinalOutputPass(GLRenderer* renderer);
	~FinalOutputPass();

	bool Init()override;
	void Destroy()override;
	void PreRun()override;
	void Run(Scene* scene)override;
	void PostRun()override;

	GLuint GetColorAttachment() { return (static_cast<FinalOutputFB*>(m_Framebuffer))->GetColorAttachment(); }
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




