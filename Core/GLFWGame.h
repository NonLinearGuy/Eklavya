#ifndef INC_APPLICATION_H
#define INC_APPLICATION_H


//C++ headers
#include<string>

//third party headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>


namespace HipHop
{
	class GLWindowContext
	{
	public:
		GLWindowContext();
		~GLWindowContext();
		bool Create(const std::string& name,int majorVersion,int minorVersion,int width,int height,bool fullScreen = false);
		void Destroy();
		void SetPosition(int x,int y);
		void SetDimension(int width,int height);
		inline int GetMajorVersion() { return m_MajorGLVersion; }
		inline int GetMinorVersion() { return m_MinorGLVersion; }
		inline int GetWidth() { return m_Width; }
		inline int GetHeight() { return m_Height; }
		inline int GetX() { return m_X; }
		inline int GetY() { return m_Y; }
		inline std::string GetName() { return m_Name; }
		inline GLFWwindow* GetWindow() { return m_Window; }
		inline float GetAspectRatio() { return m_Width / float(m_Height); }
		
	private:
		int m_X;
		int m_Y;
		int m_MinorGLVersion;
		int m_MajorGLVersion;
		int m_Width;
		int m_Height;
		bool m_IsFullScreen;
		std::string m_Name;
		GLFWwindow* m_Window;
	};

	class GLFWGame
	{
	protected:
		GLWindowContext* m_CurrentContext;
		
	public:
		GLFWGame();
		virtual ~GLFWGame();

		virtual bool Initialize(const std::string& pName,
			int pWindowWidth,
			int pWindowHeight,
			bool pFullScreen);
		virtual void Destroy();
		virtual void Tick() {}
		

		void Start();
		void GameLoop();

		inline GLWindowContext* GetCurrentContext() {return m_CurrentContext; }

		//Callbacks
		virtual void OnKeyAction(int pKey, int pAction) {}
		virtual void OnError(int pError, const char* pDesc) {}
		virtual void OnResize(int newWidth, int newHeight) {}
		virtual void OnMouseAction(int pKey, int pAction) {}
		virtual void OnCursorMoved(double pX, double pY) {}

		void CloseWindow();
		void HideMouse();
		void SwapBuffers();
	};

	class CallbackManager
	{
	public:
		static void SetCallbacks(GLFWGame* pApp);
	private:
		static GLFWGame* mApp;
		static void KeyCallback(GLFWwindow* window,
			int key,
			int scancode,
			int action,
			int mods);
		static void ErrorCallback(int error,const char* desc);
		static void WindowResizeCallback(GLFWwindow* window,int width,int height);
		static void MouseButtonCallback(GLFWwindow* window,
			int key,
			int action,
			int mods);
		static void CursorPositionCallback(GLFWwindow* window,double xPos,double yPos);
	};

}



#endif