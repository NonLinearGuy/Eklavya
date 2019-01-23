#include "RBSim.h"
#include "InputHandler.h"
#include "Renderer/Texture2D.h"
#include "Renderer/ShaderProgram.h"
#include "Renderer/Material.h"

RBSim::RBSim()
{
}

RBSim::~RBSim()
{
}

bool RBSim::Initialize(const std::string & windowName, int width, int height, bool fullScreen)
{
	GLFWGame::Initialize(windowName,width,height,fullScreen);

	m_Camera = std::make_shared<DebugCamera>(45.0f,width/float(height),0.1f,300.0f);
	m_ShaderProgram = std::make_shared<ShaderProgram>();
	m_ShaderProgram->AddAndCompile("Assets/Shaders/unlit_solids_vs.glsl",EShaderType::VERTEX);
	m_ShaderProgram->AddAndCompile("Assets/Shaders/unlit_solids_fs.glsl", EShaderType::FRAGMENT);
	m_ShaderProgram->Build();


	return true;
}

void RBSim::Tick()
{
	//Update all colliders

	//check for collision

	//render
}

void RBSim::Destroy()
{
}

void RBSim::OnKeyAction(int key, int action)
{
	InputHandler::GetInstance()->OnKeyAction(key,action);
}

void RBSim::OnMouseAction(int key, int action)
{
	InputHandler::GetInstance()->OnMouseAction(key,action);
}

void RBSim::OnCursorMoved(double x, double y)
{
	InputHandler::GetInstance()->OnCursorMove(x,y);
}


//BOX
void Box::Init(std::shared_ptr<ShaderProgram> shader)
{
}

void Box::Destroy()
{
}

void Box::Render()
{
}

//Sphere
void Sphere::Init(std::shared_ptr<ShaderProgram> shader)
{
}

void Sphere::Destroy()
{
}

void Sphere::Render()
{
}
