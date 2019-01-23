#ifndef _INC_DEBUG_CAMERA_H_
#define _INC_DEBUG_CAMERA_H_

#include <memory>
#include<glm/glm.hpp>
#include "BaseNode.h"
#include "../UserInputListener.h"

class GLRenderer;
class Scene;
class Frustum;

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

	DebugCamera(float fov,float ratio,float nearDist,float farDist);
	~DebugCamera();

	void OnCursorMove(double x, double y) override;
	void PollKeyAction() override;
	void OnKeyAction(int key,int action)override;

	void PreRender(Scene* scene)override;
	void Render(Scene* scene)override;
	void PostRender(Scene* scene)override;

	glm::mat4 GetView();
	glm::mat4 GetProjection();
	glm::mat4 GetClipMatrix();
	inline glm::vec3 GetFront() { return m_Front; }

	void UpdateCamera();

	inline glm::vec3 GetPosition(){return m_Position;}
	inline bool IsDebugEnabled() { return m_Debug; }
	inline Frustum* GetFrustum() { return m_Frustum; }

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
	glm::mat4 m_View;
	glm::mat4 m_Projection;
	Frustum* m_Frustum;
};

#endif