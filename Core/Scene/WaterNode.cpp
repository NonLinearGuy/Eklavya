#include "WaterNode.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "DebugCamera.h"
#include "../Renderer/GLRenderer.h"

WaterNode::WaterNode()
	:
	BaseNode(2123,nullptr,ERenderGroup::WATER)
{
}

WaterNode::~WaterNode()
{

}

bool WaterNode::Init()
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

	m_Shader.AddAndCompile("Assets/Shaders/water_vs.glsl",EShaderType::VERTEX);
	m_Shader.AddAndCompile("Assets/Shaders/water_fs.glsl", EShaderType::FRAGMENT);
	m_Shader.Build();

	m_DuDvMap.CreateTexture("Assets/Textures/waterDistortMap.png",true);
	m_NormalMap.CreateTexture("Assets/Textures/waterNormalMap.png",true);

	m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	m_Model = glm::scale(m_Model,glm::vec3(1000,1.0f,1000.0f));
	m_Model = glm::rotate(m_Model,-90.0f,glm::vec3(1.0f,0.0f,0.0f));
	
	m_MoveOffset = 0.0f;

	return false;
}

void WaterNode::Destroy()
{
	glDeleteVertexArrays(1,&m_VAO);
}

void WaterNode::Tick(Scene * scene, float deltaTime)
{
	m_MoveOffset += .1f * deltaTime;
}

void WaterNode::PreRender(Scene * scene)
{
	glm::mat4 projection = scene->GetCamera()->GetProjection();
	glm::mat4 view = scene->GetCamera()->GetView();

	m_Shader.Use();
	m_Shader.SetMat4("projection",projection);
	m_Shader.SetMat4("view",view);
	m_Shader.SetMat4("model",m_Model);
	m_Shader.SetVec3("cameraPosition",scene->GetCamera()->GetPosition());
	m_Shader.SetFloat("moveOffset",m_MoveOffset);
	scene->GetRenderer()->SetWaterPassValues(m_Shader);
	m_Shader.SetInt("dudvMap",4);
	m_DuDvMap.BindToUnit(GL_TEXTURE4);
	m_Shader.SetInt("normalMap",5);
	m_NormalMap.BindToUnit(GL_TEXTURE5);
}

void WaterNode::Render(Scene * scene)
{
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES,0,6);
	glBindVertexArray(0);
}

void WaterNode::PostRender(Scene * scene)
{
}
