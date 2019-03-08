#include "KeyFrame.h"


KeyFrame::KeyFrame()
	:
	m_Orientation(), m_Position(0.0f), m_TimeStamp(0.0f)
{

}

KeyFrame::KeyFrame(const aiQuaternion& orientation, const aiVector3D& position, float timeStamp)
	:
	m_TimeStamp(timeStamp)
{
	m_Orientation.x = orientation.x;
	m_Orientation.y = orientation.y;
	m_Orientation.z = orientation.z;
	m_Orientation.w = -orientation.w;

	m_Position.x = position.x;
	m_Position.y = position.z;
	m_Position.z = position.y;
}

KeyFrame::~KeyFrame()
{
}
