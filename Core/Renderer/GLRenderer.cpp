#include "GLRenderer.h"
#include "../Engine.h"
#include "Framebuffer.h"
#include "ShaderProgram.h"
#include <glad/glad.h>
#include "../Scene/Scene.h"
#include "../GLFWGame.h"

using namespace HipHop;

GLRenderer::GLRenderer(GLWindowContext* context)
	:
	m_CurrentState(new DefaultState()),
	m_Context(context)
{

}

GLRenderer::~GLRenderer()
{
}

bool GLRenderer::Initialize()
{

/*	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
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
	m_MainOutputShader.Build();*/

	//Load all the shaders

	m_Programs[EShaderProgram::SOLID] = LoadShaderProgram("solids");
	m_Programs[EShaderProgram::SKYBOX] = LoadShaderProgram("skybox");
	m_ActiveProgram = m_Programs[EShaderProgram::SOLID];
	m_ActiveProgram->Use();

	SetClearColor(0.0f,0.0f,0.0f,1.0f);

	glEnable(GL_DEPTH_TEST);

	return true;
}

void GLRenderer::Destroy()
{

}

std::shared_ptr<ShaderProgram> GLRenderer::LoadShaderProgram(const std::string& name)
{
	std::shared_ptr<ShaderProgram> newShader(new ShaderProgram());
	newShader->AddAndCompile("Assets/Shaders/" + name + "_vs.glsl", EShaderType::VERTEX);
	newShader->AddAndCompile("Assets/Shaders/" + name + "_fs.glsl", EShaderType::FRAGMENT);
	if (newShader->Build())
		return newShader;
	else
		return std::shared_ptr<ShaderProgram>();
}

void GLRenderer::RenderWorld(Scene * scene)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	scene->Render();

	SwapBuffers();
}

void GLRenderer::SetViewport(int x,int y,int w,int h)
{
	glViewport(x,y,w,h);
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

void GLRenderer::SetShaderProgram(EShaderProgram program)
{
	m_ActiveProgram = m_Programs[program];
	m_ActiveProgram->Use();
}

void GLRenderer::SetWorldMatrix(glm::mat4 & worldTransform)
{
	m_ActiveProgram->SetMat4("model",worldTransform);
}

void GLRenderer::SetViewMatrix(glm::mat4 & view)
{
	m_ActiveProgram->SetMat4("view", view);
}

void GLRenderer::SetProjectionMatrix(glm::mat4 & projection)
{
	m_ActiveProgram->SetMat4("projection", projection);
}

void GLRenderer::SwapBuffers()
{
	m_Context->SwapBuffers();
}
/*

void GLRenderer::RunAllPasses(Scene* scene)
{
	for (int i = 0; i < m_RenderGroupes.size(); ++i)
	{
		m_CurrentRenderPass = (ERenderPass)i;
		m_RenderGroupes[i]->PreRun();
		m_RenderGroupes[i]->Run(scene);
		m_RenderGroupes[i]->PostRun();
	}
}

IRenderPass* GLRenderer::GetRenderGroup(ERenderPass type)
{
	return m_RenderGroupes[(unsigned int)type];
}

void GLRenderer::RenderOutput()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SetViewport(0, 0, m_Width, m_Height);

	glBindVertexArray(m_VAO);
	m_MainOutputShader.Use();

	GLuint mainPassOutput = (static_cast<ShadowMapPass*>(m_RenderGroupes[(int)ERenderPass::SHADOW_PASS])->GetDepthAttachment());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mainPassOutput);
	m_MainOutputShader.SetInt("mainPassOutput", 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	
}


/*


// RENDER PASSES

IRenderPass::IRenderPass(ERenderPass type,GLRenderer* renderer) : m_Type(type), m_Renderer(renderer)
{
}

IRenderPass::~IRenderPass()
{
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
	
	glm::vec3 lightPosition = glm::vec3(100.0f);
	glm::mat4 projection = glm::ortho(-100.0f,100.0f,-100.0f,100.0f,-100.0f,200.0f);
	glm::mat4 view = m_Renderer->GetCameraPosition();

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
*/