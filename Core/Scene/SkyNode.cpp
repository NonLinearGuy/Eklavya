#include "SkyNode.h"

#include "../Components/GameActor.h"
#include "../Components/Transform.h"
#include "../Components/BaseRenderComponent.h"
#include "../Helpers.h"
#include "DebugCamera.h"
#include "../Engine.h"
#include "Scene.h"

using namespace HipHop;

SkyNode::SkyNode(ActorID id, BaseRenderComponent * renderComponent, ERenderGroup renderPass)
	:
	BaseNode(id,renderComponent,renderPass)
{
}

SkyNode::~SkyNode()
{
}

bool SkyNode::Init()
{

	float skyboxVertices[] = {
		// positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	m_VAO.Create(skyboxVertices,sizeof(skyboxVertices),0);
	m_VAO.SetPosPtr(3,0,0);

	std::vector<std::string> faceNames;
	faceNames.push_back("right");
	faceNames.push_back("left");
	faceNames.push_back("top");
	faceNames.push_back("bottom");
	faceNames.push_back("back");
	faceNames.push_back("front");

	m_Cubemap.Create(faceNames);

	m_ShaderProgram.AddAndCompile("Assets/Shaders/cubemap_vs.glsl",EShaderType::VERTEX);
	m_ShaderProgram.AddAndCompile("Assets/Shaders/cubemap_fs.glsl", EShaderType::FRAGMENT);
	m_ShaderProgram.Build();

	return true;
}

void SkyNode::Destroy()
{
	m_Cubemap.Delete();
}

void SkyNode::PreRender(Scene * scene)
{
	m_ShaderProgram.Use();

	glm::mat4 projection = scene->GetProjection();

	glm::mat4 view = scene->GetCamera()->GetView();
	glm::mat4 model = glm::mat4(1.0f);

	std::shared_ptr<GameActor> gameActor = scene->GetEngineRef()->GetActor(m_ActorID);
	std::shared_ptr<Transform> transform = MakeSharedPtr(gameActor->GetComponent<Transform>(Transform::s_ID));
	if (transform)
	{
		model = transform->GetModelMatrix();
	}

	m_ShaderProgram.SetMat4("projection", projection);
	m_ShaderProgram.SetMat4("view", view);
	m_ShaderProgram.SetMat4("model", model);

	m_Cubemap.BindToUnit(GL_TEXTURE0);
}

void SkyNode::Render(Scene * scene)
{
	m_VAO.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void SkyNode::PostRender(Scene * scene)
{
	
}
