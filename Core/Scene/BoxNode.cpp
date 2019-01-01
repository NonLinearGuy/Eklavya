#include "BoxNode.h"
#include "../Components/Transform.h"
#include "Scene.h"
#include <memory>
#include "../Components/GameActor.h"
#include "DebugCamera.h"
#include "../Engine.h"
#include "../Helpers.h"
#include "../Renderer/GLRenderer.h"

using namespace HipHop;

BoxNode::BoxNode(ActorID actorID,BaseRenderComponent* renderComponent, ERenderGroup renderPass)
	:
	BaseNode(actorID,renderComponent,renderPass)
{

}

BoxNode::~BoxNode()
{

}

bool BoxNode::Init()
{
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	m_VAOConfig.Create(vertices,sizeof(vertices),0);
	m_VAOConfig.SetPosPtr(3,0,sizeof(float) * 8);
	m_VAOConfig.SetNormalPtr(sizeof(float) * 3,sizeof(float) * 8);
	m_VAOConfig.SetTexPtr(sizeof(float) * 6,sizeof(float) * 8);

	m_ShaderProgram.AddAndCompile("assets/shaders/test_vs.glsl",EShaderType::VERTEX);
	m_ShaderProgram.AddAndCompile("assets/shaders/test_fs.glsl", EShaderType::FRAGMENT);
	m_ShaderProgram.Build();
	
	m_FloorTexture.CreateTexture("Assets/Textures/albedo.jpg");

	return true;
}

void BoxNode::Destroy()
{
	m_FloorTexture.Delete();
	m_VAOConfig.Destroy();
}

void BoxNode::Tick(Scene* scene,float deltaTime)
{
}

void BoxNode::PreRender(Scene* scene)
{
	std::shared_ptr<GameActor> gameActor = scene->GetEngineRef()->GetActor(m_ActorID);
	std::shared_ptr<Transform> transform = MakeSharedPtr(gameActor->GetComponent<Transform>(Transform::s_ID));
	glm::mat4 model = glm::mat4(1.0f);
	if (transform)
	{
		model = transform->GetModelMatrix();
	}

	auto currentRenderPass = scene->GetRenderer()->GetCurrentRenderPass();

	if (ERenderPass::SHADOW_PASS == currentRenderPass)
	{
		auto shadowShader = static_cast<ShadowMapPass*>(scene->GetRenderer()->GetRenderPass(ERenderPass::SHADOW_PASS))->GetShader();
		shadowShader.Use();
		shadowShader.SetMat4("model", model);
	}
	/*else
	{
		m_ShaderProgram.Use();
		glm::mat4 projection = scene->GetProjection();
		glm::mat4 view = scene->GetCamera()->GetView();
		m_ShaderProgram.SetMat4("projection", projection);
		m_ShaderProgram.SetMat4("view", view);
		m_ShaderProgram.SetMat4("model", model);
		m_ShaderProgram.SetInt("floorTexture", 0);
		m_FloorTexture.BindToUnit(GL_TEXTURE0);
	}*/
}

void BoxNode::Render(Scene* scene)
{
	m_VAOConfig.Bind();
	glDrawArrays(GL_TRIANGLES,0,36);
}
