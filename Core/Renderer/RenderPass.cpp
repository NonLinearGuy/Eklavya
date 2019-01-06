#include "RenderPass.h"
#include "Framebuffer.h"
#include "GLRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../Scene/Scene.h"
#include "../Scene/DebugCamera.h"

ShadowMapPass::ShadowMapPass(GLRenderer * renderer,int depthPrecision,int width,int height)
	:
	IRenderPass(ERenderPass::SHADOW_PASS,renderer),
	m_DepthPrecision(depthPrecision),
	m_Width(width),
	m_Height(height)
{
}

ShadowMapPass::~ShadowMapPass()
{
}

bool ShadowMapPass::Init()
{
	m_Framebuffer = new ShadowPassFB(m_DepthPrecision,m_Width,m_Height);
	m_Framebuffer->Setup();
	return false;
}

void ShadowMapPass::Destroy()
{
	m_Framebuffer->Destroy();
	delete m_Framebuffer;
}

void ShadowMapPass::PreRun()
{
	m_Framebuffer->Bind();
	m_Renderer->SetViewport(0,0,m_Width,m_Height);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMapPass::Run(Scene * scene)
{
	m_Renderer->SetShaderProgram(EShaderProgram::SHADOW);

	glm::vec3 lightPosition = scene->GetLightSource().m_Position;
	glm::mat4 projection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, -1000.0f, 1000.0f);
	glm::mat4 view = glm::lookAt(lightPosition,glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
	m_LightProjectionView = projection * view;
	m_Renderer->GetActiveProgram()->SetMat4("lightProjectionView",m_LightProjectionView);

	scene->Render(ERenderPass::SHADOW_PASS);

}

void ShadowMapPass::PostRun()
{
	m_Framebuffer->Unbind();
}

GLuint ShadowMapPass::GetDepthAttachment()
{
	auto shadowFB = static_cast<ShadowPassFB*>(m_Framebuffer);
	return shadowFB->GetDepthAttachment();
}

IRenderPass::IRenderPass(ERenderPass type, GLRenderer * renderer)
	:
	m_Type(type),m_Renderer(renderer)
{
}

IRenderPass::~IRenderPass()
{
}

WaterTexturePass::WaterTexturePass(GLRenderer * renderer, bool InvertCamera, glm::vec4 clipPlane, int width, int height)
	:
	IRenderPass(ERenderPass::WATER_TEXTURE_PASS,renderer),
	m_Width(width),
	m_Height(height),
	m_ClipPlane(clipPlane),
	m_InvertCamera(InvertCamera)
{
}

WaterTexturePass::~WaterTexturePass()
{
}

bool WaterTexturePass::Init()
{
	m_Framebuffer = new GeneralFB(4,24,m_Width,m_Height);
	m_Framebuffer->Setup();
	return true;
}

void WaterTexturePass::Destroy()
{
	m_Framebuffer->Destroy();
	delete m_Framebuffer;
}

void WaterTexturePass::PreRun()
{
	m_Framebuffer->Bind();
	m_Renderer->SetViewport(0,0,m_Width,m_Height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CLIP_DISTANCE0);
}

void WaterTexturePass::Run(Scene * scene)
{
	scene->GetCamera()->Invert(m_InvertCamera);
	m_Renderer->SetShaderProgram(EShaderProgram::WATER_PASS);
	m_Renderer->GetActiveProgram()->SetVec4("clipPlane", m_ClipPlane);
	scene->Render(ERenderPass::WATER_TEXTURE_PASS);
	scene->GetCamera()->Invert(false);
}

void WaterTexturePass::PostRun()
{
	glDisable(GL_CLIP_DISTANCE0);
	m_Framebuffer->Unbind();
}

GLuint WaterTexturePass::GetColorAttachment()
{
	return static_cast<GeneralFB*>(m_Framebuffer)->GetColorAttachment();
}
