#ifndef _INC_DEBUG_CAMERA_H_
#define _INC_DEBUG_CAMERA_H_

#include<glm/glm.hpp>
#include "../UserInputListener.h"

class DebugCamera : public UserInputListener
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

	glm::mat4 GetView();
	inline glm::vec3 GetPosition()
	{
		return m_Position;
	}

private:
	void Move(EDirection direction, float dt);

private:
	float m_Pitch;
	float m_Yaw;
	bool m_CursorStartedMoving;
	glm::mat4 m_View;
	float m_Senstivity;
	glm::vec3 m_Front;
	glm::vec3 m_Position;
	float m_LastCursorX;
	float m_LastCursorY;
	float m_Speed;
	
};

#endif