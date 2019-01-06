#ifndef _INC_DEBUG_CAMERA_H_
#define _INC_DEBUG_CAMERA_H_

#include<glm/glm.hpp>
#include "../UserInputListener.h"
#include <memory>
#include <glad/glad.h>
#include "BaseNode.h"
#include <glm/glm.hpp>

class GLRenderer;

class DebugCamera : public BaseNode,public UserInputListener
{
public:

	enum EDirection
	{
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	DebugCamera();

	void OnCursorMove(double x, double y) override;
	void PollKeyAction() override;
	void Invert(bool invert);

	glm::mat4 GetView();
	inline glm::vec3 GetPosition()
	{
		return m_Position;
	}

	bool IsDebugEnabled() { return m_Debug; }

private:
	void Move(EDirection direction, float dt);

private:
	float m_Pitch;
	float m_Yaw;
	bool m_CursorStartedMoving;
	float m_Senstivity;
	glm::vec3 m_Front;
	glm::vec3 m_Position;
	float m_LastCursorX;
	float m_LastCursorY;
	float m_Speed;
	bool m_Debug;
	bool m_InvertCamera;
	glm::mat4 m_View;
	glm::mat4 m_Projection;
};

#endif