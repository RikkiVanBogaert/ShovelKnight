#include "pch.h"
#include "Platform.h"
#include "Texture.h"

int Platform::m_NrInstances{};
Texture* Platform::m_pTexture{};

Platform::Platform(const Point2f& minPos, const Point2f& maxPos, bool isVertical) :
	Item(minPos),
	m_MinPos{minPos},
	m_MaxPos{maxPos},
	m_Speed{40},
	m_IsVertical{isVertical}
{
	++m_NrInstances;
	if (!m_pTexture) m_pTexture = new Texture{ "Resources/Images/Platform.png" };

	m_Shape.width = m_pTexture->GetWidth();
	m_Shape.height = m_pTexture->GetHeight();
}

Platform::~Platform()
{
	--m_NrInstances;
	if (m_NrInstances <= 0)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}


void Platform::Draw() const
{
	m_pTexture->Draw(Point2f(m_Shape.left, m_Shape.bottom));
}

void Platform::Update(float elapsedSec)
{
	if (!m_IsVertical)
	{
		m_Shape.left += m_Speed * elapsedSec;
		if (m_Shape.left <= m_MinPos.x)
		{
			m_Shape.left = m_MinPos.x;
			m_Speed *= -1;
		}
		else if (m_Shape.left + m_Shape.width >= m_MaxPos.x)
		{
			m_Shape.left = m_MaxPos.x - m_Shape.width;
			m_Speed *= -1;
		}
	}
	else
	{
		m_Shape.bottom += m_Speed * elapsedSec;
		if (m_Shape.bottom <= m_MinPos.y)
		{
			m_Shape.bottom = m_MinPos.y;
			m_Speed *= -1;
		}
		else if (m_Shape.bottom + m_Shape.height >= m_MaxPos.y)
		{
			m_Shape.bottom = m_MaxPos.y - m_Shape.height;
			m_Speed *= -1;
		}
	}


}

void Platform::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, float elapsedSec)
{
	Point2f top{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height };
	Point2f bottom{ actorShape.left + actorShape.width / 2, actorShape.bottom };

	Point2f left{ actorShape.left , actorShape.bottom + actorShape.height / 2 };
	Point2f right{ actorShape.left + actorShape.width , actorShape.bottom + actorShape.height / 2 };

	if (utils::IsPointInRect(top, m_Shape))
	{
		actorShape.bottom = m_Shape.bottom - actorShape.height;
		actorVelocity.y = 0;
	}

	if (utils::IsPointInRect(bottom, m_Shape))
	{
		actorShape.bottom = m_Shape.bottom + m_Shape.height;
		actorVelocity.y = 0;
		if (!m_IsVertical) actorVelocity.x = m_Speed;
		else actorVelocity.y = m_Speed;
	}

	//sides
	if (utils::IsPointInRect(left, m_Shape))
	{
		actorShape.left = m_Shape.left + m_Shape.width;
		actorVelocity.x = 0;
	}

	if (utils::IsPointInRect(right, m_Shape))
	{
		actorShape.left = m_Shape.left - actorShape.width;
		actorVelocity.x = 0;
	}
}

bool Platform::IsOnGround( const Rectf& actorShape) const
{
	Point2f bottom{ actorShape.left + actorShape.width / 2, actorShape.bottom };

	if (utils::IsPointInRect(bottom, m_Shape))
	{
		return true;
	}
	return false;
}

void Platform::IsHit()
{}

bool Platform::GetIsDead() const
{
	return false;
}

bool Platform::GetIsActive() const
{
	return true;
}
