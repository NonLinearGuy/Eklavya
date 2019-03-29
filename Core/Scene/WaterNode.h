#pragma once

#include "BaseNode.h"
#include "../Renderer/VertexArrayObject.h"
#include "../AssetManager/ShaderProgram.h"
#include "../AssetManager/Texture2D.h"

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
	std::weak_ptr<ShaderProgram> m_Shader;
	std::weak_ptr<Texture2D> m_DuDvMap; 
	std::weak_ptr<Texture2D> m_NormalMap;
	glm::mat4 m_Model;
	float m_MoveOffset;

};