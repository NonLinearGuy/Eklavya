#ifndef _INC_CAMERA_H_
#define _INC_CAMERA_H_

#include<glm/glm.hpp>
#include "UserInputListener.h"
#include "GameObject.h"

class Camera : public UserInputListener
{
public:

	enum EDirection
	{
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	Camera(std::shared_ptr<GameObject> pTarget);

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
	std::shared_ptr<GameObject> m_Target;
	
};

#endif