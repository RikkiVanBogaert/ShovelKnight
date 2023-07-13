#include "pch.h"
#include "Camera.h"
#include "SVGParser.h"

Camera::Camera(float width, float height):
	m_Width{width},
	m_Height{height},
	m_Scale{3.2f},
	m_CameraState{-1}, // -1 for spawning the first level
	m_TempCameraState{},
	m_NewCameraPos{},
	m_CameraPos{},
	m_isPlayerInBounds{true},
	m_IsTransitioning{false}
{
	m_Width /= m_Scale;
	m_Height /= m_Scale;

	SVGParser::GetVerticesFromSvgFile("Resources/Images/Level/LevelStages.svg", m_Vertices);
}

void Camera::Transform(const Rectf& target)
{
	ChangeState(target);

	ChangeCameraPosition(target);
	glTranslatef(-m_CameraPos.x * m_Scale, -m_CameraPos.y * m_Scale, 0);
	glScalef(m_Scale, m_Scale, 1);
}

void Camera::ChangeCameraPosition(const Rectf& target)
{
	switch (m_CameraState)
	{
	case 0:
		m_NewCameraPos = Point2f(Track(target).x, 608);
		break;
	case 1:
		m_NewCameraPos = Point2f(1632, 608);
		break;
	case 2:
		m_NewCameraPos = Point2f(2032, 608);
		break;
	case 3:
		m_NewCameraPos = Point2f(2032, 816);
		break;
	case 4:
		m_NewCameraPos = Point2f(2430, 816);
		break;
	case 5:
		m_NewCameraPos = Point2f(2832, 816);
		break;
	case 6:
		m_NewCameraPos = Point2f(2832, 608);
		break;
	case 7:
		m_NewCameraPos = Point2f(2832, 400);
		break;
	case 8:
		m_NewCameraPos = Point2f(2832, 192);
		break;
	case 9:
		m_NewCameraPos = Point2f(2432, 192);
		break;
	case 10:
		m_NewCameraPos = Point2f(Track(target).x, 192);
		break;
	case 11:
		m_NewCameraPos = Point2f(4064, 192);
		break;
	case 12:
		m_NewCameraPos = Point2f(4064, 400);
		break;
	case 13:
		m_NewCameraPos = Point2f(Track(target).x, 400);
		break;
	case 14:
		m_NewCameraPos = Point2f(4064, 605);
		break;
	case 15:
		m_NewCameraPos = Point2f(4064, 816);
		break;
	case 16:
		m_NewCameraPos = Point2f(Track(target).x, 816);
		break;
	case 17:
		m_NewCameraPos = Point2f(5248, 816);
		break;
	case 18:
		m_NewCameraPos = Point2f(5648, 816);
		break;
	case 19:
		m_NewCameraPos = Point2f(5648, 1024);
		break;
	case 20:
		m_NewCameraPos = Point2f(5648, 1232);
		break;
	case 21:
		m_NewCameraPos = Point2f(Track(target).x, 1232);
		break;
	case 22:
		m_NewCameraPos = Point2f(6064, 1232);
		break;
	default:
		m_NewCameraPos = Point2f(Track(target).x, Track(target).y);
		break;
	}
}

void Camera::ChangeState(const Rectf& target)
{
	const Point2f playerPos{ target.left + target.width / 2, target.bottom + target.height / 2 };
	m_isPlayerInBounds = false;
	for (int i{}; i < m_Vertices.size(); ++i)
	{
		if (utils::IsPointInPolygon(playerPos, m_Vertices[i]))
		{
			m_CameraState = i;
			m_isPlayerInBounds = true;
		}
	}
}

Point2f Camera::Track(const Rectf& target)
{
	Point2f pos{};
	pos.x = target.left + target.width / 2 - m_Width / 2;
	pos.y = target.bottom + target.height / 2 - m_Height / 2; 
	Clamp(pos); 
	return pos;
}

void Camera::Clamp(Point2f& bottomLeftPos)
{
	//make all the track x cameraPoss clamps
	Rectf boundaries{};
	int currentStage{};
	switch (m_CameraState)
	{
	case 0:
		currentStage = 0;
		break;
	case 10:
		currentStage = 10;
		break;
	case 13:
		currentStage = 13;
		break;
	case 16:
		currentStage = 16;
		break;
	case 21:
		currentStage = 21;
		break;
	}
	if(m_Vertices[currentStage][0].x < m_Vertices[currentStage][2].x) 
		boundaries.left = m_Vertices[currentStage][0].x;
	else 
		boundaries.left = m_Vertices[currentStage][2].x;
	boundaries.width = abs(m_Vertices[currentStage][2].x - m_Vertices[currentStage][0].x);

	if (bottomLeftPos.x + m_Width > boundaries.left + boundaries.width)
	{
		bottomLeftPos.x = boundaries.left + boundaries.width - m_Width;
	}
	if (bottomLeftPos.x < boundaries.left )
	{
		bottomLeftPos.x = boundaries.left;
	}
}

void Camera::Transition(float elapsedSec)
{
	const float cameraSpeed{400 * elapsedSec};
	const float margin{ 20 };
	if (!m_IsTransitioning)
	{
		m_CameraPos = m_NewCameraPos;
	}
	if (m_IsTransitioning && m_CameraState >= 0)
	{
		if (m_NewCameraPos.x - m_CameraPos.x > margin)
			m_CameraPos.x += cameraSpeed;
		else if (m_NewCameraPos.x - m_CameraPos.x < -margin)
			m_CameraPos.x -= cameraSpeed;
		else if (m_NewCameraPos.y - m_CameraPos.y > margin)
			m_CameraPos.y += cameraSpeed;
		else if (m_NewCameraPos.y - m_CameraPos.y < -margin)
			m_CameraPos.y -= cameraSpeed;
		else
		{
			m_IsTransitioning = false;
			m_TempCameraState = m_CameraState;
			m_CameraPos = m_NewCameraPos;
		}
	}
	else
	{
		m_IsTransitioning = false;
	}

	//for testing
	/*m_IsTransitioning = false;
	m_CameraPos = m_NewCameraPos;
	m_TempCameraState = m_CameraState;*/
}

bool Camera::HasStageChanged()
{
	if (m_CameraState == m_TempCameraState)
		return false;
	else if (m_CameraState == -1) // for spawning the first level objects
	{
		m_CameraState = 0;
		return true;
	}
	else
	{
		m_IsTransitioning = true;
		return true;
	}
}

int Camera::GetCurrentStage() const
{
	return m_CameraState;
}

bool Camera::GetIsPlayerInBounds() const
{
	return m_isPlayerInBounds;
}

bool Camera::GetIsTransitioning() const
{
	return m_IsTransitioning;
}

Vector2f Camera::GetTransform() const
{
	return Vector2f(-m_CameraPos.x * m_Scale, -m_CameraPos.y * m_Scale);
}

float Camera::GetScale() const
{
	return m_Scale;
}

bool Camera::GetIsGameDone() const
{
	if (m_CameraState == m_Vertices.size() - 1)
		return true;
	else return false;
}