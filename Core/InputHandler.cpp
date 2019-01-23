#include "InputHandler.h"
#include <GLFW/glfw3.h>

#include "UserInputListener.h"

InputHandler::InputHandler()
{
	for (int index = 0; index < 1024; ++index)
		m_KeyStates[index] = false;

	m_Listeners.reserve(10);
}

void InputHandler::PollKeyActions()
{
	for (auto listener : m_Listeners)
		listener->PollKeyAction();
}

void InputHandler::OnMouseAction(int key, int action)
{
	for (auto listener : m_Listeners)
		listener->OnMouseAction(key,action);
}

void InputHandler::OnKeyAction(int key, int action)
{
	if (action == GLFW_PRESS) m_KeyStates[key] = true;
	if (action == GLFW_RELEASE) m_KeyStates[key] = false;

	for (auto listener : m_Listeners)
		listener->OnKeyAction(key,action);
}

void InputHandler::OnCursorMove(double x, double y)
{
	for (auto listener : m_Listeners)
		listener->OnCursorMove(x,y);
}

void InputHandler::AddListener(UserInputListener* pListener)
{
	m_Listeners.push_back(pListener);
}

void InputHandler::RemoveListener(UserInputListener* pListener)
{
	auto iter = std::find(m_Listeners.begin() , m_Listeners.end() , pListener);
	if (iter != m_Listeners.end())
		m_Listeners.erase(iter); //"Iter" cannot be used because Its invalidated
}


InputHandler::~InputHandler()
{
	m_Listeners.clear();
}
