#pragma once

#include "GLFWGame.h"
#include "Physics/Collider.h"
#include <list>
#include "Scene/DebugCamera.h"
#include <memory>

class Texture2D;
class ShaderProgram;
class VertexArrayObject;

class RBSim : public GLFWGame
{
public:

	RBSim();
	~RBSim();

	bool Initialize(const std::string& windowName, int width, int height, bool fullScreen)override;
	void Tick() override;
	void Destroy()override;

	void OnKeyAction(int key, int action)override;
	void OnMouseAction(int key, int action)override;
	void OnCursorMoved(double x, double y)override;
private:

	std::shared_ptr<DebugCamera> m_Camera;
	std::shared_ptr<ShaderProgram> m_ShaderProgram;
};


class IRenderable
{
public:
	virtual void Init(std::shared_ptr<ShaderProgram> shader) = 0;
	virtual void Render() = 0;
	virtual void Destroy() = 0;
protected:
	std::shared_ptr<Material> m_Material
};

class Box : public IRenderable,public BoxCollider
{
public:
	void Init(std::shared_ptr<ShaderProgram> shader)override;
	void Destroy()override;
	void Render()override;
private:
	VertexArrayObject* m_VAO;
};


class SphereVAO;

class Sphere : public SphereCollider
{
public:
	void Init(std::shared_ptr<ShaderProgram> shader);
	void Destroy();
	void Render();
private:
	SphereVAO* m_VAO;
};