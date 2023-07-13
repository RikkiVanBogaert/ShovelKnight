#include "pch.h"
#include "Blorb.h"
#include "Texture.h"

int Blorb::m_NrInstances{};
Texture* Blorb::m_pTexture{};

Blorb::Blorb(const Point2f& pos) :
	Enemy(pos, 2, Vector2f(-10, 0))
{
	++m_NrInstances;
	if (!m_pTexture) m_pTexture = new Texture{ "Resources/Images/Blorb.png" };

	m_SrcRect.width = m_pTexture->GetWidth() / 4;
	m_SrcRect.height = m_pTexture->GetHeight() / 2;
	m_SrcRect.left = 0;
	m_SrcRect.bottom = m_SrcRect.height;

	m_Shape.width = m_SrcRect.width;
	m_Shape.height = m_SrcRect.height;
}

Blorb::~Blorb()
{
	--m_NrInstances;
	if (m_NrInstances <= 0)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

void Blorb::Draw() const
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

void Blorb::Update(float elapsedSec)
{
	Enemy::Update(elapsedSec);

	m_SrcRect.left = m_AnimFrame * m_SrcRect.width;
	ChangeSprites(elapsedSec);
	Enemy::InHit(elapsedSec);

	if (m_ActionState == ActionState::idle)
		m_ActionState = ActionState::moving;
}

void Blorb::ChangeSprites(float elapsedSec)
{
	int amountFrames{4};
	int fps{5};

	switch (m_ActionState)
	{
	case ActionState::moving:
		m_SrcRect.bottom = m_SrcRect.height;
		fps = 5;
		break;
	case ActionState::poof:
		amountFrames = 5;
		fps = 5;
		m_SrcRect.width = m_pPoofTxt->GetWidth() / amountFrames;
		m_SrcRect.height = m_pPoofTxt->GetHeight();
		m_SrcRect.bottom = m_pPoofTxt->GetHeight();
		break;
	case ActionState::getHit:
		fps = 10;
		m_SrcRect.bottom = m_pTexture->GetHeight();
		break;
	}

	UpdateSprites(elapsedSec, amountFrames, fps);

}

Rectf Blorb::GetHitBox() const
{
	Rectf hitBox{};
	const float offset{ 3 };
	hitBox.left = m_Shape.left + offset;
	hitBox.bottom = m_Shape.bottom;
	hitBox.width = m_Shape.width - offset;
	hitBox.height = m_Shape.height;

	return hitBox;
}