#include "pch.h"
#include "Bird.h"
#include "Texture.h"

int Bird::m_NrInstances{};
Texture* Bird::m_pTexture{};

Bird::Bird(const Point2f& pos, const Point2f& flyDistance, const Vector2f& speed, bool goingRight) :
	Enemy(pos, 2, speed),
	m_MinPos{pos.x, pos.y}
{
	++m_NrInstances;
	if (!m_pTexture) m_pTexture = new Texture{ "Resources/Images/Bird.png" };

	m_SrcRect.width = m_pTexture->GetWidth() / 3;
	m_SrcRect.height = m_pTexture->GetHeight() / 2;
	m_SrcRect.left = 0;
	m_SrcRect.bottom = m_SrcRect.height;

	m_Shape.width = m_SrcRect.width;
	m_Shape.height = m_SrcRect.height;

	if (goingRight) m_MaxPos = Point2f(pos.x + flyDistance.x, pos.y + flyDistance.y);
	else m_MaxPos = Point2f(pos.x - flyDistance.x, pos.y - flyDistance.y);

	m_IsGoingRight = goingRight;
}

Bird::~Bird()
{
	--m_NrInstances;
	if (m_NrInstances <= 0)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

void Bird::Draw() const
{
	glPushMatrix();
	if (!m_IsGoingRight)
	{
		glTranslatef(m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2, 0);
		glScalef(-1, 1, 1);
		glTranslatef(-m_Shape.left - m_Shape.width / 2, -m_Shape.bottom - m_Shape.height / 2, 0);
	}
	if (m_ActionState != ActionState::poof) m_pTexture->Draw(m_Shape, m_SrcRect);
	else m_pPoofTxt->Draw(m_Shape, m_SrcRect);
	glPopMatrix();
}

void Bird::Update(float elapsedSec)
{

	if (m_ActionState == ActionState::moving)
	{
		m_Shape.left += m_Velocity.x * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;
	}
	SwitchSides();
	DieWhenPoofDone();

	m_SrcRect.left = m_AnimFrame * m_SrcRect.width;
	ChangeSprites(elapsedSec);
	InHit(elapsedSec);

	if (m_ActionState == ActionState::idle)
		m_ActionState = ActionState::moving;
}

void Bird::SwitchSides()
{
	const Point2f mid{ m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2 };
	if (m_MaxPos.x - m_MinPos.x >= 20)
	{
		if (mid.x <= m_MinPos.x)
		{
			m_Shape.left = m_MinPos.x - m_Shape.width / 2;
			m_Velocity.x *= -1;
			m_IsGoingRight = true;
		}
		else if (mid.x >= m_MaxPos.x)
		{
			m_Shape.left = m_MaxPos.x - m_Shape.width / 2;
			m_Velocity.x *= -1;
			m_IsGoingRight = false;
		}
	}

	if (abs(m_MaxPos.y - m_MinPos.y) >= 20)
	{
		if (mid.y <= m_MinPos.y)
		{
			m_Shape.bottom = m_MinPos.y - m_Shape.height / 2;
			m_Velocity.y *= -1;
		}
		else if (mid.y >= m_MaxPos.y)
		{
			m_Shape.bottom = m_MaxPos.y - m_Shape.height / 2;
			m_Velocity.y *= -1;
		}
	}
}

void Bird::ChangeSprites(float elapsedSec)
{
	int amountFrames{3};
	int fps{5};

	switch (m_ActionState)
	{
	case ActionState::moving:
		m_SrcRect.bottom = m_SrcRect.height;
		fps = 5;
		break;
	case ActionState::getHit:
		m_SrcRect.bottom = m_SrcRect.height * 2;
		fps = 10;
		break;
	case ActionState::poof:
		amountFrames = 5;
		m_SrcRect.width = m_pPoofTxt->GetWidth() / amountFrames;
		m_SrcRect.height = m_pPoofTxt->GetHeight();
		m_SrcRect.bottom = m_pPoofTxt->GetHeight();
		fps = 5;
		break;
	}

	UpdateSprites(elapsedSec, amountFrames, fps);

}

Rectf Bird::GetHitBox() const
{
	Rectf hitBox{};
	const float offset{ 3 };
	hitBox.left = m_Shape.left + offset;
	hitBox.bottom = m_Shape.bottom;
	hitBox.width = m_Shape.width - offset;
	hitBox.height = m_Shape.height - offset;

	return hitBox;
}