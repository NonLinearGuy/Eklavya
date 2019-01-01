#include "GLRenderer.h"
#include "../Engine.h"
#include "Framebuffer.h"
#include "ShaderProgram.h"
#include <glad/glad.h>
#include "../Scene/Scene.h"

using namespace HipHop;

GLRenderer::GLRenderer(int width,int height)
	:
	m_CurrentState(new DefaultState()),
	m_Width(width),m_Height(height)
{
	glEnable(GL_DEPTH_TEST);
}

GLRenderer::~GLRenderer()
{
}

bool GLRenderer::Initialize(Engine* engine)
{
	m_EngineRef = engine;

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	  // positions   // texCoords
	  -1.0f,  1.0f,  0.0f, 1.0f,
	  -1.0f, -1.0f,  0.0f, 0.0f,
	   1.0f, -1.0f,  1.0f, 0.0f,

	  -1.0f,  1.0f,  0.0f, 1.0f,
	   1.0f, -1.0f,  1.0f, 0.0f,
	   1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));

	glBindVertexArray(0);

	m_MainOutputShader.AddAndCompile("Assets/Shaders/main_output_vs.glsl", EShaderType::VERTEX);
	m_MainOutputShader.AddAndCompile("Assets/Shaders/main_output_fs.glsl", EShaderType::FRAGMENT);
	m_MainOutputShader.Build();

	auto shadowMapPass = new ShadowMapPass(this);
	shadowMapPass->Init();
	m_RenderPasses.push_back(shadowMapPass);

	auto finalOutputPass = new FinalOutputPass(this);
	finalOutputPass->Init();
	m_RenderPasses.push_back(finalOutputPass);

	SetClearColor(0.0f,0.0f,0.0f,1.0f);
	glEnable(GL_DEPTH_TEST);

	m_CurrentRenderPass = ERenderPass::MIN;
	return true;
}

void GLRenderer::Destroy()
{

}

void GLRenderer::SetViewport(int x,int y,int w,int h)
{
	glViewport(x,y,w,h);
}

void GLRenderer::ClearBuffers()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderer::SetClearColor(float r, float g, float b, float a)
{
	glClearColor(r,g,b,a);
}

void GLRenderer::ChangeState(IRenderState * NewRenderState)
{
	m_CurrentState->Exit();
	delete m_CurrentState;
	m_CurrentState = NewRenderState;
	m_CurrentState->Enter();
}

void GLRenderer::SwapBuffers()
{
	m_EngineRef->SwapBuffers();
}

void GLRenderer::RunAllPasses(Scene* scene)
{
	for (int i = 0; i < m_RenderPasses.size(); ++i)
	{
		m_CurrentRenderPass = (ERenderPass)i;
		m_RenderPasses[i]->PreRun();
		m_RenderPasses[i]->Run(scene);
		m_RenderPasses[i]->PostRun();
	}
}

IRenderPass* GLRenderer::GetRenderPass(ERenderPass type)
{
	return m_RenderPasses[(unsigned int)type];
}

void GLRenderer::RenderOutput()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SetViewport(0, 0, m_Width, m_Height);

	glBindVertexArray(m_VAO);
	m_MainOutputShader.Use();

	GLuint mainPassOutput = (static_cast<ShadowMapPass*>(m_RenderPasses[(int)ERenderPass::SHADOW_PASS])->GetDepthAttachment());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mainPassOutput);
	m_MainOutputShader.SetInt("mainPassOutput", 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	
}




// RENDER PASSES

IRenderPass::IRenderPass(ERenderPass type,GLRenderer* renderer) : m_Type(type), m_Renderer(renderer)
{
}

IRenderPass::~IRenderPass()
{
}

FinalOutputPass::FinalOutputPass(GLRenderer* renderer)
	:
	IRenderPass(ERenderPass::FINAL_OUTPUT_PASS,renderer)
{
}

FinalOutputPass::~FinalOutputPass()
{

}

bool FinalOutputPass::Init()
{
	m_Framebuffer = new FinalOutputFB(4,16,m_Renderer->GetWidth(),m_Renderer->GetHeight());
	m_Framebuffer->Setup();
	return true;
}

void FinalOutputPass::Destroy()
{
	m_Framebuffer->Destroy();
}

void FinalOutputPass::PreRun()
{
	m_Framebuffer->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0,0,m_Renderer->GetWidth(),m_Renderer->GetHeight());
}

void FinalOutputPass::Run(Scene* scene)
{
	scene->Render();
}

void FinalOutputPass::PostRun()
{
	m_Framebuffer->Unbind();
}

ShadowMapPass::ShadowMapPass(GLRenderer * renderer)
	:
	IRenderPass(ERenderPass::SHADOW_PASS,renderer)
{
}

ShadowMapPass::~ShadowMapPass()
{
	
}

bool ShadowMapPass::Init()
{
	m_Framebuffer = new ShadowPassFB(24,1024,1024);
	m_Framebuffer->Setup();

	m_ShadowShader.AddAndCompile("Assets/Shaders/shadow_map_vs.glsl",EShaderType::VERTEX);
	m_ShadowShader.AddAndCompile("Assets/Shaders/shadow_map_fs.glsl", EShaderType::FRAGMENT);
	m_ShadowShader.Build();

	return true;
}

void ShadowMapPass::Destroy()
{
	m_Framebuffer->Destroy();
}

void ShadowMapPass::PreRun()
{
	m_Framebuffer->Bind();
	glViewport(0, 0, 1024, 1024);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	glm::vec3 lightPosition = m_Renderer->GetCameraPosition();
	glm::mat4 projection = glm::ortho(-100.0f,100.0f,-100.0f,100.0f,-1000.0f,1000.0f);
	glm::mat4 view = glm::lookAt(lightPosition,glm::vec3(0.0f),glm::vec3(0.0f,1.0f,0.0f));

	glm::mat4 lightProjectionView = projection * view;
	
	m_ShadowShader.Use();
	m_ShadowShader.SetMat4("lightProjectionView",lightProjectionView);
}

void ShadowMapPass::Run(Scene * scene)
{
	scene->Render();
}

void ShadowMapPass::PostRun()
{
	m_Framebuffer->Unbind();
}
