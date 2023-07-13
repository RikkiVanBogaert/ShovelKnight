#include "pch.h"
#include "Beeto.h"
#include "Texture.h"

int Beeto::m_NrInstances{};
Texture* Beeto::m_pTexture{};

Beeto::Beeto(const Point2f& pos):
	Enemy(pos, 1, Vector2f(20, 0))
{
	++m_NrInstances;
	if (!m_pTexture) m_pTexture = new Texture{ "Resources/Images/Beeto.png" };

	m_SrcRect.width = m_pTexture->GetWidth() / 4;
	m_SrcRect.height = m_pTexture->GetHeight() / 2;
	m_SrcRect.left = 0;
	m_SrcRect.bottom = m_SrcRect.height;

	m_Shape.width = m_SrcRect.width;
	m_Shape.height = m_SrcRect.height;
}

Beeto::~Beeto()
{
	--m_NrInstances;
	if (m_NrInstances <= 0)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

void Beeto::Draw() const
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

void Beeto::Update(float elapsedSec)
{
	Enemy::Update(elapsedSec);

	m_SrcRect.left = m_AnimFrame * m_SrcRect.width;
	ChangeSprites(elapsedSec);
	InHit(elapsedSec);
}

void Beeto::ChangeSprites(float elapsedSec)
{
	bool isLooping{true};
	int amountFrames{};

	switch (m_ActionState)
	{
	case ActionState::moving:
		amountFrames = 4;
		break;
	case ActionState::getHit:
		m_SrcRect.bottom = m_pTexture->GetHeight();
		amountFrames = 2;
		break;
	case ActionState::poof:
		amountFrames = 5;
		m_SrcRect.width = m_pPoofTxt->GetWidth() / amountFrames;
		m_SrcRect.height = m_pPoofTxt->GetHeight();
		m_SrcRect.bottom = m_pPoofTxt->GetHeight();
		break;
	}

	UpdateSprites(elapsedSec, amountFrames);

}

void Beeto::InHit(float elapsedSec, float stunTime)
{
	Vector2f knockBack{ 7, 70 };
	if (m_ActionState == ActionState::getHit)
	{
		if (m_KnightIsRight) knockBack.x *= -1;

		m_Shape.left += knockBack.x * elapsedSec;
		m_Shape.bottom += knockBack.y * elapsedSec;

		m_StunTimer += elapsedSec;

		if (m_IsOnGround && m_StunTimer >= 0.2f)
		{
			m_ActionState = ActionState::poof;
		}

	}
}

Rectf Beeto::GetHitBox() const
{
	Rectf hitBox{};
	const float offset{ 3 };
	hitBox.left = m_Shape.left + offset;
	hitBox.bottom = m_Shape.bottom;
	hitBox.width = m_Shape.width - offset;
	hitBox.height = m_Shape.height;

	return hitBox;
}

