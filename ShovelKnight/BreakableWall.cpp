#include "pch.h"
#include "BreakableWall.h"
#include "Gem.h"
#include "Platter.h"
#include "Skeleton.h"
#include "Blorb.h"
#include "Texture.h"
#include "SoundEffect.h"
#include "SoundStream.h"

int BreakableWall::m_NrInstances{};
Texture* BreakableWall::m_pWallTexture{ nullptr };
Texture* BreakableWall::m_pExplosionTexture{ nullptr };
SoundStream* BreakableWall::m_pWallBreak{ nullptr };

BreakableWall::BreakableWall(const Point2f& pos, int typeWall):
	Interactable(pos),
	m_TypeWall{typeWall}
{
	++m_NrInstances;
	if (!m_pWallTexture) m_pWallTexture = new Texture{ "Resources/Images/Wall.png" };
	if (!m_pExplosionTexture) m_pExplosionTexture = new Texture{ "Resources/Images/Explosion.png" };
	if (!m_pWallBreak) m_pWallBreak = new SoundStream{ "Resources/Sounds/WallDestroy.wav" };
	GiveTypeWall(typeWall);
	m_Shape.width = m_SrcRect.width;
	m_Shape.height = m_SrcRect.height;
}

void BreakableWall::GiveTypeWall(int typeWall)
{
	switch (typeWall)
	{
	case 0:
		m_SrcRect.left = 0;
		m_SrcRect.bottom = m_pWallTexture->GetHeight();
		m_SrcRect.width = 160;
		m_SrcRect.height = 49;
		break;
	case 1:
		m_SrcRect.left = 160;
		m_SrcRect.bottom = m_pWallTexture->GetHeight();
		m_SrcRect.width = 128;
		m_SrcRect.height = 49;
		break;
	case 2:
		m_SrcRect.left = 225;
		m_SrcRect.bottom = 48;
		m_SrcRect.width = 64;
		m_SrcRect.height = 48;
		break;
	case 3:
		m_SrcRect.left = 176;
		m_SrcRect.bottom = 48;
		m_SrcRect.width = 48;
		m_SrcRect.height = 48;
		break;
	case 4:
		m_SrcRect.left = 0;
		m_SrcRect.bottom = 48;
		m_SrcRect.width = 178;
		m_SrcRect.height = 48;
		break;
	case 5:
		m_SrcRect.left = 288;
		m_SrcRect.bottom = m_pWallTexture->GetHeight();
		m_SrcRect.width = 144;
		m_SrcRect.height = 96;
		break;
	case 6:
		m_SrcRect.left = 433;
		m_SrcRect.bottom = 48;
		m_SrcRect.width = 80;
		m_SrcRect.height = 48;
		break;
	case 7:
		m_SrcRect.left = 432;
		m_SrcRect.bottom = 80;
		m_SrcRect.width = 48;
		m_SrcRect.height = 32;
		break;
	}
}

BreakableWall::~BreakableWall()
{
	--m_NrInstances;

	if (m_NrInstances <= 0)
	{
		delete m_pWallTexture;
		m_pWallTexture = nullptr;
		delete m_pExplosionTexture;
		m_pExplosionTexture = nullptr;

		delete m_pWallBreak;
		m_pWallBreak = nullptr;
	}
}

void BreakableWall::Draw() const
{
	if (m_State == State::active)
	{
		m_pWallTexture->Draw(m_Shape, m_SrcRect);
	}
	else if (m_State == State::inactive)
	{
		m_pExplosionTexture->Draw(m_Shape, m_SrcRect);
	}
}

void BreakableWall::Update(float elapsedSec)
{
	if (m_State == State::inactive)
	{
		m_SrcRect.width = m_pExplosionTexture->GetWidth() / 7;
		m_SrcRect.height = m_pExplosionTexture->GetHeight();
		m_SrcRect.left = m_AnimFrame * m_SrcRect.width;
		m_SrcRect.bottom = m_pExplosionTexture->GetHeight();
		ChangeFrame(elapsedSec, 7, 6, false);
	}
}

void BreakableWall::SpawnGems(std::vector<Item*>& items, bool isKnightRight)
{
	Point2f pos{m_Shape.left, m_Shape.bottom + m_Shape.height / 2};
	const Vector2f velocity{ 0, 20 };
	switch (m_TypeWall)
	{
	case 0:
		items.push_back(new Gem{ Point2f(pos.x + m_Shape.width / 5, pos.y), velocity });
		items.push_back(new Gem{ Point2f(pos.x + 2 * m_Shape.width / 5, pos.y), velocity });
		items.push_back(new Gem{ Point2f(pos.x + 3 * m_Shape.width / 5, pos.y), velocity });
		items.push_back(new Gem{ Point2f(pos.x + 4 * m_Shape.width / 5, pos.y), velocity });
		break;
	case 2:
		items.push_back(new Gem{ Point2f(pos.x + 3 * m_Shape.width / 4, pos.y), velocity });
		break;
	case 3:
		items.push_back(new Gem{ Point2f(pos.x + m_Shape.width / 2, pos.y), Vector2f(0,0), true });
		break;
	case 4:
		items.push_back(new Gem{ Point2f(pos.x + m_Shape.width / 5, pos.y), velocity });
		items.push_back(new Gem{ Point2f(pos.x + 2 * m_Shape.width / 5, pos.y), velocity });
		items.push_back(new Gem{ Point2f(pos.x + 4 * m_Shape.width / 5, pos.y), velocity, true});
		break;
	case 6:
		items.push_back(new Gem{ Point2f(pos.x + m_Shape.width / 3, pos.y), Vector2f(0,0), true});
		break;
	case 7:
		items.push_back(new Platter{ Point2f(pos.x + m_Shape.width / 4, pos.y - m_Shape.height / 2) });
		break;
	}
}

void BreakableWall::SpawnEnemies(std::vector<Enemy*>& enemies)
{
	Point2f pos{ m_Shape.left, m_Shape.bottom + m_Shape.height / 2 };
	switch (m_TypeWall)
	{
	case 1:
		enemies.push_back(new Blorb{ Point2f(pos.x + 3 * m_Shape.width / 5, pos.y)});
		enemies.push_back(new Blorb{ Point2f(pos.x + 4 * m_Shape.width / 5, pos.y) });
		break;
	case 4:
		enemies.push_back(new Skeleton{Point2f(pos.x + 3 * m_Shape.width / 5, m_Shape.bottom)});
		break;
	case 5:
		enemies.push_back(new Skeleton{ Point2f(pos.x + 3 * m_Shape.width / 5, m_Shape.bottom) });
		break;

	}
}

void BreakableWall::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, bool isNPC)
{
	Point2f top{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height };
	Point2f bottom{ actorShape.left + actorShape.width / 2, actorShape.bottom };

	Point2f left{ actorShape.left , actorShape.bottom + actorShape.height / 3 };
	Point2f right{ actorShape.left + actorShape.width , actorShape.bottom + actorShape.height / 3 };

	//Point2f mid{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height * 0.8f };

	if (utils::IsPointInRect(top, m_Shape))
	{
		actorShape.bottom = m_Shape.bottom - actorShape.height;
		actorVelocity.y = 0;

	}

	if (utils::IsPointInRect(bottom, m_Shape))
	{
		actorShape.bottom = m_Shape.bottom + m_Shape.height;
		actorVelocity.y = 0;
	}

	//sides
	if (utils::IsPointInRect(left, m_Shape))
	{
		actorShape.left = m_Shape.left + m_Shape.width;
		if (!isNPC) actorVelocity.x = 0;
		else actorVelocity.x *= -1;
	}

	if (utils::IsPointInRect(right, m_Shape))
	{
		actorShape.left = m_Shape.left - actorShape.width;
		if (!isNPC) actorVelocity.x = 0;
		else actorVelocity.x *= -1;
	}
}

void BreakableWall::IsHit()
{
	if (m_State == State::active)
	{
		m_State = State::inactive;
		m_pWallBreak->Play(false);
	}
}

bool BreakableWall::IsOnGround(const Rectf& actorShape) const
{
	Point2f bottom{ actorShape.left + actorShape.width / 2, actorShape.bottom };

	if (utils::IsPointInRect(bottom, m_Shape))
	{
		return true;
	}
	return false;
}