#ifndef _INC_RENDER_PASS_H_
#define _INC_RENDER_PASS_H_

#include <glad/glad.h>
#include <glm/glm.hpp>

class GLRenderer;
class Scene;
class IFramebuffer;
class ShadowPassFB;


enum class ERenderPass
{
	MIN = 0,
	SHADOW_PASS = MIN,
	WATER_TEXTURE_PASS,
	MAIN_PASS,
	MAX
};


class IRenderPass
{
public:
	IRenderPass(ERenderPass type, GLRenderer* renderer);
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
	ShadowMapPass(GLRenderer* renderer,int depthPrecision,int width,int height);
	~ShadowMapPass();

	bool Init()override;
	void Destroy()override;
	void PreRun()override;
	void Run(Scene* scene)override;
	void PostRun()override;

	glm::mat4 GetLightSpaceMatrix() { return m_LightProjectionView; }
	GLuint GetDepthAttachment();
private:
	int m_DepthPrecision;
	int m_Width;
	int m_Height;
	glm::mat4 m_LightProjectionView;
};


class WaterTexturePass : public IRenderPass
{
public:
	WaterTexturePass(GLRenderer* renderer, bool InvertCamera,glm::vec4 clipPlane,int width,int height);
	~WaterTexturePass();

	bool Init()override;
	void Destroy()override;
	void PreRun()override;
	void Run(Scene* scene)override;
	void PostRun()override;

	GLuint GetColorAttachment();

private:
	int m_DepthPrecision;
	int m_Width, m_Height;
	glm::vec4 m_ClipPlane;
	bool m_InvertCamera;
};

#endif