#include <glad/glad.h>
#include "GLFWGame.h"
#include <stdio.h>
#include "Utils/Logger.h"


namespace HipHop
{

	//GLWindowContext
	GLWindowContext::GLWindowContext() : m_Window(nullptr)
	{
	}

	GLWindowContext::~GLWindowContext()
	{
		
	}

	bool GLWindowContext::Create(const std::string & name, int majorVersion, int minorVersion, int width, int height, bool fullScreen)
	{
		m_Name = name;
		m_X = m_Y = 0;
		m_Width = width;
		m_Height = height;
		m_MajorGLVersion = majorVersion;
		m_MinorGLVersion = minorVersion;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
		glfwWindowHint(GLFW_SAMPLES, 4);

		m_Window = glfwCreateWindow(
			width,
			height,
			name.c_str(),
			fullScreen ? glfwGetPrimaryMonitor() : nullptr,
			nullptr
		);

		if (!m_Window)
		{
			fprintf(stderr,"\n failed to create glfw window");
			return false;
		}

		return true;
	}

	void GLWindowContext::SetPosition(int x, int y)
	{
		if (m_Window)
			glfwSetWindowPos(m_Window,x,y);
	}

	void GLWindowContext::SetDimension(int width, int height)
	{
		if (m_Window)
			glfwSetWindowSize(m_Window,width,height);
	}

	void GLWindowContext::Destroy()
	{
		if (m_Window)
		{
			glfwDestroyWindow(m_Window);
			m_Window = nullptr;
		}
	}


	GLFWGame::GLFWGame() 
	{
	}



	// GLFWGame
	
	bool GLFWGame::Initialize(const std::string& pName,
			int pWndWidth,
			int pWndHeight,
			bool pFullScreen)
		{
			if (!glfwInit())
			{
				fprintf(stderr,"\n Failed to initialize GLFW");
				return false;
			}

			Logger::GetInstance()->Init("============HIP HOP 1.0=============");

			m_CurrentContext = new GLWindowContext();
			bool success = m_CurrentContext->Create(pName,4,3,pWndWidth,pWndHeight,pFullScreen);
			if (!success)
				return false;

			glfwMakeContextCurrent(m_CurrentContext->GetWindow());
			CallbackManager::SetCallbacks(this);
			/*GLint flags;
			glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

			if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
			{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(debug_output_callback, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);
			}
			else
			{
			glfwDestroyWindow(mWindow);
			glfwTerminate();
			throw std::runtime_error(
			"failed to get debugger!"
			);
			}*/

			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				fprintf(stderr,"Glad : Failed to load glLoader");
				return false;
			}

			return true;
	}

	void GLFWGame::Destroy()
	{
		if (m_CurrentContext)
		{
			delete m_CurrentContext;
			m_CurrentContext = nullptr;
		}
		glfwTerminate();
	}

	GLFWGame::~GLFWGame()
	{
		
	}

	void GLFWGame::HideMouse()
	{
		glfwSetInputMode(m_CurrentContext->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void GLFWGame::CloseWindow()
	{
		glfwSetWindowShouldClose(m_CurrentContext->GetWindow(), true);
	}

	void GLFWGame::SwapBuffers()
	{
		glfwSwapBuffers(m_CurrentContext->GetWindow());
	}

	void GLFWGame::Start()
	{
		GameLoop();
	}


	void GLFWGame::GameLoop()
	{
		while (!glfwWindowShouldClose(m_CurrentContext->GetWindow()))
		{
			glfwPollEvents();
			this->Tick();
		}
	}


	//CALLBACK MANAGER

	void CallbackManager::SetCallbacks(GLFWGame* pApp)
	{
		mApp = pApp;
		GLFWwindow* window = mApp->GetCurrentContext()->GetWindow();
		glfwSetKeyCallback(window, KeyCallback);
		glfwSetFramebufferSizeCallback(window, WindowResizeCallback);
		glfwSetMouseButtonCallback(window, MouseButtonCallback);
		glfwSetCursorPosCallback(window, CursorPositionCallback);
		glfwSetErrorCallback(ErrorCallback);
	}

	void CallbackManager::KeyCallback(GLFWwindow* window,
		int key,
		int scancode,
		int action,
		int mods)
	{
		if (mApp) mApp->OnKeyAction(key, action);
	}


	void CallbackManager::ErrorCallback(int error,
		const char* desc)
	{
		if (mApp) mApp->OnError(error, desc);
	}


	void CallbackManager::WindowResizeCallback(GLFWwindow* window,
		int width,
		int height)
	{
		if (mApp) mApp->OnResize(width, height);
	}

	void CallbackManager::MouseButtonCallback(GLFWwindow* window,
		int key,
		int action,
		int mods)
	{
		if (mApp) mApp->OnMouseAction(key, action);
	}

	void CallbackManager::CursorPositionCallback(GLFWwindow* window,
		double xPos,
		double yPos)
	{
		if (mApp) mApp->OnCursorMoved(xPos, yPos);
	}

	GLFWGame* CallbackManager::mApp = nullptr;



}