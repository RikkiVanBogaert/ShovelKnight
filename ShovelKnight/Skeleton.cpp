#include "pch.h"
#include "Skeleton.h"
#include "Texture.h"

int Skeleton::m_NrInstances;
Texture* Skeleton::m_pTexture{};

Skeleton::Skeleton(const Point2f& pos):
	Enemy(pos, 2),
	m_AttackTimer{}
{
	++m_NrInstances;
	if (!m_pTexture) m_pTexture = new Texture{ "Resources/Images/Skeleton.png" };

	m_SrcRect.width = m_pTexture->GetWidth() / 4;
	m_SrcRect.height = m_pTexture->GetHeight() / 4;
	m_SrcRect.left = 0;
	m_SrcRect.bottom = m_SrcRect.height;

	m_Shape.width = m_SrcRect.width;
	m_Shape.height = m_SrcRect.height;
}

Skeleton::~Skeleton()
{
	--m_NrInstances;
	if (m_NrInstances <= 0)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

void Skeleton::Draw() const
{
	glPushMatrix();
	if (!m_IsGoingRight)
	{
		glTranslatef(m_Shape.left + m_Shape.width / 3, m_Shape.bottom + m_Shape.height / 3, 0);
		glScalef(-1, 1, 1);
		glTranslatef(-m_Shape.left - m_Shape.width / 3, -m_Shape.bottom - m_Shape.height / 3, 0);
		//  /3 cus otherwise its not clean
	}
	if (m_ActionState != ActionState::poof) m_pTexture->Draw(m_Shape, m_SrcRect);
	else m_pPoofTxt->Draw(m_Shape, m_SrcRect);
	glPopMatrix();

}

void Skeleton::Update(float elapsedSec)
{
	Enemy::Update(elapsedSec);

	Enemy::m_SrcRect.left = m_AnimFrame * m_SrcRect.width;
	ChangeSprites(elapsedSec);
	InHit(elapsedSec, 0.2f);
}

void Skeleton::ChangeSprites(float elapsedSec)
{
	const float attackTime{1};

	m_SrcRect.left = m_AnimFrame * m_SrcRect.width;
	switch (m_ActionState)
	{
	case ActionState::idle:
		m_SrcRect.bottom = m_SrcRect.height * 2;
		UpdateSprites(elapsedSec, 2);
		break;
	case ActionState::moving:
		m_SrcRect.bottom = m_SrcRect.height;
		UpdateSprites(elapsedSec, 4);
		break;
	case ActionState::attack:
		m_AttackTimer += elapsedSec;
		if (m_AttackTimer < attackTime)
		{
			m_AnimFrame = 0;
			m_SrcRect.bottom = m_SrcRect.height * 3;
		}
		else if (m_AttackTimer < attackTime + 0.5f)
		{
			m_AnimFrame = 1;
		}
		else
		{
			m_AttackTimer = 0;
			m_ActionState = ActionState::idle;
		}

		break;
	case ActionState::getHit:
		m_SrcRect.bottom = m_SrcRect.height * 4;
		UpdateSprites(elapsedSec, 2, 8);
		break;
	case ActionState::poof:
		m_SrcRect.width = m_pPoofTxt->GetWidth() / 5;
		m_SrcRect.height = m_pPoofTxt->GetHeight();
		m_SrcRect.bottom = m_pPoofTxt->GetHeight();
		UpdateSprites(elapsedSec, 5);
		break;
	}

}

void Skeleton::UpdateState(float elapsedSec, const Rectf& actorShape)
{
	const float startMovingDistance{ 200 };
	const float attackDistance{36};
	const float speed{ 40 };
	const float playerMid{ actorShape.left + actorShape.width / 2 };
	const float shapeMid{ m_Shape.left + m_Shape.width / 3 };
	const float currDistance{ abs(shapeMid - playerMid) };

	switch (m_ActionState)
	{
	case ActionState::idle:
		if (currDistance < startMovingDistance)
			m_ActionState = ActionState::moving;
		break;

	case ActionState::moving:
		if (currDistance > startMovingDistance)
			m_ActionState = ActionState::idle;
		else if (currDistance < attackDistance)
			m_ActionState = ActionState::attack;

		if (actorShape.left > m_Shape.left)
		{
			m_IsGoingRight = true;
			m_Velocity.x = speed;
		}
		else
		{
			m_IsGoingRight = false;
			m_Velocity.x = -speed;
		}
		break;
	}
}

void Skeleton::InHit(float elapsedSec, float stunTime)
{
	Enemy::InHit(elapsedSec, stunTime);
	if (m_ActionState == ActionState::getHit)
	{
		const float knockBack{ 7 * elapsedSec };

		if (m_KnightIsRight) m_Shape.left += knockBack;
		else m_Shape.left -= knockBack;
	}
}

Rectf Skeleton::GetHitBox() const
{
	Rectf hitBox{};
	const int offset{ 15 };

	hitBox.bottom = m_Shape.bottom;
	hitBox.height = 28;

	if (m_ActionState == ActionState::attack && m_AnimFrame == 1)
	{
		hitBox.width = 44;
	}
	else 
	{
		hitBox.width = 29;
	}

	if (m_IsGoingRight)
	{
		hitBox.left = m_Shape.left + m_Shape.width / 3 - offset;
	}
	else
	{
		hitBox.left = m_Shape.left + m_Shape.width / 3 - hitBox.width + offset;
	}

	return hitBox;
}
