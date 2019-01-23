#include "BoxNode.h"
#include "../Components/Transform.h"
#include "Scene.h"
#include <memory>
#include "../Components/GameActor.h"
#include "DebugCamera.h"
#include "../Engine.h"
#include "../Helpers.h"
#include "../Renderer/GLRenderer.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Material.h"
#include "BoundingVolume.h"
#include "../Components/BaseRenderComponent.h"



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
		// Back face
		-0.5f, -0.5f, -0.5f, 0.0f,0.0f,-1.0f, 0.0f, 0.0f, // Bottom-left
		 0.5f,  0.5f, -0.5f, 0.0f,0.0f,-1.0f,  1.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f, 0.0f,0.0f,-1.0f,  1.0f, 0.0f, // bottom-right         
		 0.5f,  0.5f, -0.5f, 0.0f,0.0f,-1.0f,  1.0f, 1.0f, // top-right
		-0.5f, -0.5f, -0.5f, 0.0f,0.0f,-1.0f,  0.0f, 0.0f, // bottom-left
		-0.5f,  0.5f, -0.5f, 0.0f,0.0f,-1.0f,  0.0f, 1.0f, // top-left
		// Front face
		-0.5f, -0.5f,  0.5f, 0.0f,0.0f,1.0f,  0.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,  0.0f,0.0f,1.0f, 1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f,  0.5f, 0.0f,0.0f,1.0f,  1.0f, 1.0f, // top-right
		 0.5f,  0.5f,  0.5f,0.0f,0.0f,1.0f,   1.0f, 1.0f, // top-right
		-0.5f,  0.5f,  0.5f, 0.0f,0.0f,1.0f,  0.0f, 1.0f, // top-left
		-0.5f, -0.5f,  0.5f, 0.0f,0.0f,1.0f,  0.0f, 0.0f, // bottom-left
		// Left face
		-0.5f,  0.5f,  0.5f, -1.0f,0.0f,0.0f,  1.0f, 0.0f, // top-right
		-0.5f,  0.5f, -0.5f,  -1.0f,0.0f,0.0f, 1.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f,  -1.0f,0.0f,0.0f, 0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, -0.5f,  -1.0f,0.0f,0.0f, 0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  -1.0f,0.0f,0.0f, 0.0f, 0.0f, // bottom-right
		-0.5f,  0.5f,  0.5f,  -1.0f,0.0f,0.0f, 1.0f, 0.0f, // top-right
		// Right face
		 0.5f,  0.5f,  0.5f,  1.0f,0.0f,0.0f, 1.0f, 0.0f, // top-left
		 0.5f, -0.5f, -0.5f,  1.0f,0.0f,0.0f, 0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,  1.0f,0.0f,0.0f, 1.0f, 1.0f, // top-right         
		 0.5f, -0.5f, -0.5f,  1.0f,0.0f,0.0f, 0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,  1.0f,0.0f,0.0f, 1.0f, 0.0f, // top-left
		 0.5f, -0.5f,  0.5f,  1.0f,0.0f,0.0f, 0.0f, 0.0f, // bottom-left     
		// Bottom face
		-0.5f, -0.5f, -0.5f,  0.0f,-1.0f,0.0f,  0.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,  0.0f,-1.0f,0.0f, 1.0f, 1.0f, // top-left
		 0.5f, -0.5f,  0.5f,  0.0f,-1.0f,0.0f, 1.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,  0.0f,-1.0f,0.0f, 1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f,-1.0f,0.0f, 0.0f, 0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f,  0.0f,-1.0f,0.0f, 0.0f, 1.0f, // top-right
		// Top face
		-0.5f,  0.5f, -0.5f,  0.0f,1.0f,0.0f, 0.0f, 1.0f, // top-left
		 0.5f,  0.5f,  0.5f,  0.0f,1.0f,0.0f, 1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,  0.0f,1.0f,0.0f, 1.0f, 1.0f, // top-right     
		 0.5f,  0.5f,  0.5f,   0.0f,1.0f,0.0f,1.0f, 0.0f, // bottom-right
		-0.5f,  0.5f, -0.5f,  0.0f,1.0f,0.0f, 0.0f, 1.0f, // top-left
		-0.5f,  0.5f,  0.5f,  0.0f,1.0f,0.0f, 0.0f, 0.0f  // bottom-left        
	};


m_VAOConfig.Create(vertices,sizeof(vertices),0);
	m_VAOConfig.SetPosPtr(3,0,sizeof(float) * 8);
	m_VAOConfig.SetNormalPtr(sizeof(float) * 3,sizeof(float) * 8);
	m_VAOConfig.SetTexPtr(sizeof(float) * 6,sizeof(float) * 8);

	m_Material = new Material();
	if (!m_AlbedoName.empty())
	{
		auto texture = std::make_shared<Texture2D>();
		texture->CreateTexture("Assets/Textures/" + m_AlbedoName);
		m_Material->SetAlbedoMap(texture);
		m_Material->SetDiffuse(glm::vec3(1.0f));
		m_Material->SetAmbient(glm::vec3(.5f));
		m_Material->SetUseColor(false);
	}
	else
	{
		m_Material->SetDiffuse(m_Color * glm::vec3(1.0f));
		m_Material->SetAmbient(m_Color * glm::vec3(.5f));
		m_Material->SetUseColor(true);
	}

	if (!m_NormalName.empty())
	{
		auto texture = std::make_shared<Texture2D>();
		texture->CreateTexture("Assets/Textures/" + m_NormalName);
		m_Material->SetNormalMap(texture);
		m_Material->SetDiffuse(glm::vec3(1.0f));
		m_Material->SetAmbient(glm::vec3(.5f));
		m_Material->SetUseColor(false);
	}

	m_Material->SetOpacity(m_Opacity);

	return true;
}

void BoxNode::Destroy()
{
	delete m_Material;
	m_VAOConfig.Destroy();
}

void BoxNode::Tick(Scene* scene,float deltaTime)
{
}

void BoxNode::Render(Scene* scene)
{
	m_VAOConfig.Bind();
	glDrawArrays(GL_TRIANGLES,0,36);
	RenderChildren(scene);
}

