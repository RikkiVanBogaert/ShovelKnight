#include "pch.h"
#include "Chicken.h"
#include "Texture.h"

int Chicken::m_NrInstances{};
Texture* Chicken::m_pTexture{};

Chicken::Chicken(const Point2f& pos) :
	PickUp(pos),
	m_Velocity{0, 100},
	m_IsOnGround{}
{
	++m_NrInstances;
	if (!m_pTexture) m_pTexture = new Texture{ "Resources/Images/Chicken.png" };

	m_Shape.width = m_pTexture->GetWidth();
	m_Shape.height = m_pTexture->GetHeight();


	m_State = State::spawning;
}

Chicken::~Chicken()
{
	--m_NrInstances;
	if (m_NrInstances <= 0)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

void Chicken::Draw() const
{
	if (m_State == State::active || m_State == State::spawning)
	{
		m_pTexture->Draw(m_Shape);
	}
	else if (m_State == State::poof)
	{
		m_pPoofTexture->Draw(m_Shape, m_SrcRect);
	}
}

void Chicken::Update(float elapsedSec)
{
	LilJump(elapsedSec);
	PickUp::Update(elapsedSec);
}

void Chicken::LilJump(float elapsedSec)
{
	if (m_Velocity.y != 0 && !m_IsOnGround)
	{
		const float gravity{ -500 };

		m_Velocity.y += gravity * elapsedSec;
		m_Shape.left += m_Velocity.x * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;
	}
	else if (m_IsOnGround && m_State == State::spawning) m_State = State::active;
}

void Chicken::SetIsOnGround(bool isOnGround)
{
	m_IsOnGround = isOnGround;
}

Rectf& Chicken::GetLevelCollisionShape()
{
	return m_Shape;
}

Vector2f& Chicken::GetVelocity()
{
	return m_Velocity;
}