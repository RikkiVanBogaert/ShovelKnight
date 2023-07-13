#include "pch.h"
#include "Enemy.h"
#include "Gem.h"
#include "Texture.h"
#include "utils.h"

int Enemy::m_NrInstancesPoof{};
Texture* Enemy::m_pPoofTxt{};

Enemy::Enemy(const Point2f& pos, int health, const Vector2f& velocity):
	m_Shape{ pos.x, pos.y, 0, 0 },
	m_ActionState{ ActionState::moving },
	m_TempState{},
	m_AnimTime{ 0 },
	m_AnimFrame{ 0 },
	m_Gravity{ -200 },
	m_Velocity{velocity},
	m_IsGoingRight{ true },
	m_KnightIsRight{ true },
	m_IsOnGround{ false },
	m_StunTimer{},
	m_IsDead{false},
	m_Health{health},
	m_HasSpawnedGems{false}
{
	++m_NrInstancesPoof;
	if (!m_pPoofTxt) m_pPoofTxt = new Texture{ "Resources/Images/EnemyPoof.png" };

}

Enemy::~Enemy()
{
	--m_NrInstancesPoof;
	if (m_NrInstancesPoof <= 0)
	{
		delete m_pPoofTxt;
		m_pPoofTxt = nullptr;
	}
}


void Enemy::Update(float elapsedSec)
{
	m_Velocity.y += m_Gravity * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;

	if (m_ActionState == ActionState::moving)
	{
		m_Shape.left += m_Velocity.x * elapsedSec;

		if (m_Velocity.x > 0) m_IsGoingRight = true;
		else m_IsGoingRight = false;
	
	}
	DieWhenPoofDone();
}

void Enemy::UpdateSprites(float elapsedSec,  const int amountFrames, int fps)
{
	if (m_ActionState != m_TempState)
	{
		m_AnimFrame = 0;
		m_TempState = m_ActionState;
	}

	m_AnimTime += elapsedSec;

	if (m_AnimTime >= 1.0f / fps)
	{
		++m_AnimFrame;
		m_AnimTime = 0;
	}
    
	m_AnimFrame %= amountFrames;
}

void Enemy::GetHit()
{
	--m_Health;
	m_ActionState = ActionState::getHit;
}

void Enemy::InHit(float elapsedSec, float stunTime)
{
	if (m_ActionState == ActionState::getHit)
	{
		m_StunTimer += elapsedSec;
		if (m_StunTimer >= stunTime)
		{
			m_StunTimer = 0;
			if (m_Health > 0)
				m_ActionState = ActionState::idle;
			else
				m_ActionState = ActionState::poof;
		}
	}
}

void Enemy::DieWhenPoofDone()
{
	if (m_ActionState == ActionState::poof && m_AnimFrame == 4)
	{
		m_IsDead = true;
	}
}

void Enemy::SpawnGems(std::vector<Item*>& items)
{
	if (!m_HasSpawnedGems && m_ActionState == ActionState::poof)
	{
		Point2f spawnPos{ };
		spawnPos.x = m_Shape.left + m_Shape.width / 2;
		spawnPos.y = m_Shape.bottom + m_Shape.height;
		Vector2f velocity{ float(rand() % 51 + 70), float(rand() % 41 + 140)};

		items.push_back(new Gem{ spawnPos, velocity });
		velocity.x *= -1;
		items.push_back(new Gem{ spawnPos, velocity });
		m_HasSpawnedGems = true;
	}
}

void Enemy::SetKnightIsRight(const bool isKnightRight)
{
	m_KnightIsRight = isKnightRight;
}

void Enemy::SetIsOnGround(bool isOnGround)
{
	m_IsOnGround = isOnGround;
}

Rectf Enemy::GetHitBox() const
{
	return m_Shape;
}

Rectf& Enemy::GetShape()
{
	return m_Shape;
}

Vector2f& Enemy::GetVelocity()
{
	return m_Velocity;
}

bool Enemy::GetIsDying() const
{
	if (m_ActionState == ActionState::poof ||
		m_ActionState == ActionState::getHit)
		return true;
	else return false;
}

bool Enemy::GetIsDead() const
{
	return m_IsDead;
}