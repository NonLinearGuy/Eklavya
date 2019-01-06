#pragma once

#include "BaseNode.h"
#include "../Renderer/VertexArrayObject.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"

class Scene;

class WaterNode : public BaseNode
{
public:
	WaterNode();
	~WaterNode();
	bool Init()override;
	void Destroy()override;
	void Tick(Scene* scene,float deltaTime)override;
	void PreRender(Scene* scene)override;
	void Render(Scene* scene)override;
	void PostRender(Scene* scene)override;
private:
	GLuint m_VAO;
	ShaderProgram m_Shader;
	Texture2D m_DuDvMap; 
	Texture2D m_NormalMap;
	glm::mat4 m_Model;
	float m_MoveOffset;

};