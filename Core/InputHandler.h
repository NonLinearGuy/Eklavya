#pragma once

#include <vector>
#include <memory>

class UserInputListener;

class InputHandler
{
public:

	static InputHandler* GetInstance()
	{
		static InputHandler *instance = new InputHandler();
		return instance;
	}

	InputHandler();

	void PollKeyActions();
	void OnMouseAction(int key,int action);
	void OnKeyAction(int key,int action);
	void OnCursorMove(double x,double y);

	void AddListener(UserInputListener* pListener);
	void RemoveListener(UserInputListener* pListener);

	inline bool KeyHasPressed(int key)
	{
		return m_KeyStates[key];
	}

	~InputHandler();
private:
	std::vector< UserInputListener* > m_Listeners;
	bool m_KeyStates[1024];
};

