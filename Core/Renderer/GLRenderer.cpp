#include "GLRenderer.h"
#include "../AssetManager/ShaderProgram.h"
#include "RenderPass.h"
#include <glad/glad.h>
#include "../Scene/Scene.h"
#include "../GLFWGame.h"
#include "../Engine.h"
#include "VertexArrayObject.h"
#include "../AssetManager/AssetManager.h"


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

	m_MainOutputShader = AssetManager::GetInstance().GetAsset<ShaderProgram>("main_output");
	
	//Load all the shaders

	m_Programs[EShaderProgram::SOLID] = AssetManager::GetInstance().GetAsset<ShaderProgram>("solids");
	m_Programs[EShaderProgram::SKYBOX] = AssetManager::GetInstance().GetAsset<ShaderProgram>("skybox");
	m_Programs[EShaderProgram::SHADOW] = AssetManager::GetInstance().GetAsset<ShaderProgram>("shadow_map");
	m_Programs[EShaderProgram::WATER_PASS] = AssetManager::GetInstance().GetAsset<ShaderProgram>("water_pass");
	m_Programs[EShaderProgram::OUTLINED] = AssetManager::GetInstance().GetAsset<ShaderProgram>("outlined");
	m_Programs[EShaderProgram::UNLIT_SOLID] = AssetManager::GetInstance().GetAsset<ShaderProgram>("unlit_solids");
	m_Programs[EShaderProgram::CONTACTS] = AssetManager::GetInstance().GetAsset<ShaderProgram>("world_point");
	m_Programs[EShaderProgram::ANIMATED_SOLID] = AssetManager::GetInstance().GetAsset<ShaderProgram>("animated_solids");

	m_ActiveProgram = m_Programs[EShaderProgram::SOLID];
	m_ActiveProgram->Use();


	auto shadowPass = new ShadowMapPass(this,32,1024 * 3.0f, 1024 * 3.0f);
	shadowPass->Init();
	auto refractionPass = new WaterTexturePass(this,false,glm::vec4(0.0f,-1.0f,0.0f,10.0f),m_Context->GetWidth(),m_Context->GetHeight());
	refractionPass->Init();
	////auto reflectionPass = new WaterTexturePass(this, true, glm::vec4(0.0f, 1.0f, 0.0f, 20.0f), m_Context->GetWidth(), m_Context->GetHeight());
	//reflectionPass->Init();
	
	m_RenderPasses.push_back(shadowPass);
	m_RenderPasses.push_back(refractionPass);
	//m_RenderPasses.push_back(reflectionPass);

	SetClearColor(0.0f,0.0f,0.0f,1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	return true;
}

void GLRenderer::Destroy()
{
	m_Programs[EShaderProgram::SOLID].reset();
	m_Programs[EShaderProgram::SKYBOX].reset();
	m_Programs[EShaderProgram::SHADOW].reset();
	m_Programs[EShaderProgram::WATER_PASS].reset();
	m_Programs[EShaderProgram::OUTLINED].reset();
	m_Programs[EShaderProgram::UNLIT_SOLID].reset();
	m_Programs[EShaderProgram::CONTACTS].reset();
	m_Programs[EShaderProgram::ANIMATED_SOLID].reset();
	
	m_MainOutputShader.reset();

}



void GLRenderer::RunAllPasses(Scene* scene)
{
	for (auto pass : m_RenderPasses)
	{
		pass->PreRun();
		pass->Run(scene);
		pass->PostRun();
	}
}

void GLRenderer::RenderOutput()
{
	glm::mat4 projection = glm::ortho(0.0f,float(m_Context->GetWidth()),float(m_Context->GetHeight()),0.0f,-1.0f,1.0f);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f,200.0f,0.0f));
	model = glm::scale(model,glm::vec3(150,100,1.0f));
	
	m_MainOutputShader->Use();
	GLuint texture = static_cast<WaterTexturePass*>(m_RenderPasses[2])->GetColorAttachment();
	m_MainOutputShader->SetInt("mainOutputPass", 0);
	m_MainOutputShader->SetMat4("model",model);
	m_MainOutputShader->SetMat4("projection", projection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,texture);
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES,0,6);
	glBindVertexArray(0);
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
	m_ActiveProgram->SetStatus(false);
	m_ActiveProgram = m_Programs[program];
	m_ActiveProgram->Use();
	m_ActiveProgram->SetStatus(true);
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

void GLRenderer::SetShadowPassValues()
{
	glm::mat4 lightSpace = static_cast<ShadowMapPass*>(m_RenderPasses[0])->GetLightSpaceMatrix();
	m_ActiveProgram->SetMat4("lightProjectionView",lightSpace);
	m_ActiveProgram->SetInt("shadowPassOutput",1);
	glActiveTexture(GL_TEXTURE1);
	GLuint texture = static_cast<ShadowMapPass*>(m_RenderPasses[0])->GetDepthAttachment();
	glBindTexture(GL_TEXTURE_2D,texture);
}

void GLRenderer::SetWaterPassValues(std::shared_ptr<ShaderProgram> shader)
{
	GLuint refractionTexture = static_cast<WaterTexturePass*>(m_RenderPasses[1])->GetColorAttachment();
	//GLuint reflectionTexture = static_cast<WaterTexturePass*>(m_RenderPasses[2])->GetColorAttachment();
	shader->SetInt("refractionTexture",2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,refractionTexture);
//	shader.SetInt("reflectionTexture", 3);
//	glActiveTexture(GL_TEXTURE3);
	//glBindTexture(GL_TEXTURE_2D, reflectionTexture);
}

void GLRenderer::SetLightValues(glm::vec3 viewSpacePos, LightSource* light)
{
	m_ActiveProgram->SetVec3("light.position",viewSpacePos);
	m_ActiveProgram->SetVec3("light.diffuse", light->m_Diffuse);
	m_ActiveProgram->SetVec3("light.specular", light->m_Specular);
	m_ActiveProgram->SetVec3("light.ambient", light->m_Ambient);
}