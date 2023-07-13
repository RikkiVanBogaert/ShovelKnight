#include "pch.h"
#include "Rock.h"
#include "Gem.h"
#include "Texture.h"

int Rock::m_NrInstancesBig{};
int Rock::m_NrInstancesSmall{};
Texture* Rock::m_pBigRockTexture{}, *Rock::m_pBigPopTexture{},
*Rock::m_pSmallRockTexture{}, *Rock::m_pSmallPopTexture{};

Rock::Rock(const Point2f& pos, bool isBig, bool spawnGem):
	Interactable(pos),
	m_IsBig{isBig},
	m_SpawnsGem{spawnGem}
{
	if (isBig)
	{
		++m_NrInstancesBig;
		if (!m_pBigRockTexture) m_pBigRockTexture = new Texture{ "Resources/Images/RockBig.png" };
		if (!m_pBigPopTexture) m_pBigPopTexture = new Texture{ "Resources/Images/RockBigPop.png" };
		m_Shape.width = m_pBigRockTexture->GetWidth();
		m_Shape.height = m_Shape.width;
	}
	else 
	{
		++m_NrInstancesSmall;
		if (!m_pSmallRockTexture) m_pSmallRockTexture = new Texture{ "Resources/Images/RockSmall.png" };
		if (!m_pSmallPopTexture) m_pSmallPopTexture = new Texture{ "Resources/Images/RockSmallPop.png" };
		m_Shape.width = m_pSmallRockTexture->GetWidth();
		m_Shape.height = m_Shape.width;
	}

}

Rock::~Rock()
{
	if (m_IsBig)
	{
		--m_NrInstancesBig;
		if (m_NrInstancesBig <= 0)
		{
			delete m_pBigRockTexture;
			m_pBigRockTexture = nullptr;
			delete m_pBigPopTexture;
			m_pBigPopTexture = nullptr;
		}
	}
	else
	{
		--m_NrInstancesSmall;
		if (m_NrInstancesSmall <= 0)
		{
			delete m_pSmallRockTexture;
			m_pSmallRockTexture = nullptr;
			delete m_pSmallPopTexture;
			m_pSmallPopTexture = nullptr;
		}
	}
}


void Rock::Draw() const
{
	if (m_IsBig)
	{
		if (m_State == State::active)
			m_pBigRockTexture->Draw(m_Shape);
		else if (m_State == State::inactive)
			m_pBigPopTexture->Draw(m_Shape, m_SrcRect);
	}
	else
	{
		if (m_State == State::active)
			m_pSmallRockTexture->Draw(m_Shape);
		else if (m_State == State::inactive)
			m_pSmallPopTexture->Draw(m_Shape, m_SrcRect);
	}
}

void Rock::Update(float elapsedSec)
{
	Point2f clipSize{};

	if (m_State == State::inactive)
	{
		if (m_IsBig)
		{
			clipSize.x = m_pBigPopTexture->GetWidth() / 3;
			clipSize.y = m_pBigPopTexture->GetHeight();

			ChangeFrame(elapsedSec, 2, 5, false);
		}
		else
		{
			clipSize.x = m_pSmallPopTexture->GetWidth() / 3;
			clipSize.y = m_pSmallPopTexture->GetHeight();

			ChangeFrame(elapsedSec, 2, 5, false);
		}

		m_SrcRect.left = clipSize.x * m_AnimFrame;
		m_SrcRect.bottom = clipSize.y;
		m_SrcRect.width = clipSize.x;
		m_SrcRect.height = clipSize.y;
	}
}

void Rock::SpawnGems(std::vector<Item*>& items, bool isKnightRight)
{
	if (m_SpawnsGem)
	{
		Vector2f velocity{ 0, 70 };
		Point2f spawnPos{ m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2 };
		items.push_back(new Gem{spawnPos, velocity});
	}
}

void Rock::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, bool isNPC)
{
	Point2f top{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height };
	Point2f bottom{ actorShape.left + actorShape.width / 2, actorShape.bottom };

	Point2f left{ actorShape.left , actorShape.bottom + actorShape.height / 3 };
	Point2f right{ actorShape.left + actorShape.width , actorShape.bottom + actorShape.height / 3 };

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

bool Rock::IsOnGround(const Rectf& actorShape) const
{
	Point2f bottom{ actorShape.left + actorShape.width / 2, actorShape.bottom };

	if (utils::IsPointInRect(bottom, m_Shape))
	{
		return true;
	}
	return false;
}