#include "pch.h"
#include "Ladder.h"
#include "SVGParser.h"
#include "Texture.h"

Ladder::Ladder():
	m_pLadderTexture{ new Texture{"Resources/Images/Level/Level1_L2.png"} },
	m_CurrentLadderIndex{},
	m_IsOnTop{false}
{
	SVGParser::GetVerticesFromSvgFile("Resources/Images/Level/Ladders.svg", m_Vertices);
}

Ladder::~Ladder()
{
	delete m_pLadderTexture;
	m_pLadderTexture = nullptr;
}

void Ladder::Draw() const
{
	m_pLadderTexture->Draw();
}

void Ladder::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, const bool isClimbing)
{
	if (m_IsOnTop && !isClimbing)
	{
		Point2f bottom{ actorShape.left + actorShape.width / 2, actorShape.bottom };
		Point2f mid{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height * 0.8f };
		utils::HitInfo hitInfo1{};


		for (int i{}; i < m_Vertices.size(); ++i)
		{
			if (utils::Raycast(m_Vertices[i], mid, bottom, hitInfo1))
			{
				actorShape.bottom = hitInfo1.intersectPoint.y;
				actorVelocity.y = 0;

			}
		}
	}

	//check if actor is on top of ladder
	float highestYLadder{};
	if (m_Vertices[m_CurrentLadderIndex][0].y > m_Vertices[m_CurrentLadderIndex][2].y)
		highestYLadder = m_Vertices[m_CurrentLadderIndex][0].y;
	else 
		highestYLadder = m_Vertices[m_CurrentLadderIndex][2].y;

	if (actorShape.bottom + 5 > highestYLadder)
	{
		m_IsOnTop = true;
	}
	else m_IsOnTop = false;
}

bool Ladder::IsOnGround(const Rectf& actorShape, const bool isClimbing) const
{
	if (m_IsOnTop && !isClimbing)
	{
		Point2f top{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 2 };
		Point2f bottom{ actorShape.left + actorShape.width / 2, actorShape.bottom };

		utils::HitInfo hitInfo{};
		for (int i{}; i < m_Vertices.size(); ++i)
		{
			if (utils::Raycast(m_Vertices[i], top, bottom, hitInfo))
			{
				return true;
			}
		}
	}
	return false;
}

bool Ladder::IsOnLadder(const Rectf& actorShape, const Vector2f& actorVelocity)
{
	const Point2f mid{ actorShape.left + actorShape.width / 2,
	actorShape.bottom + 6 };
	const Circlef playerCircle{mid, 6};

	for (int i{}; i < m_Vertices.size(); ++i)
	{

		if (utils::IsOverlapping( m_Vertices[i], playerCircle))
		{
			m_CurrentLadderIndex = i;
			return true;
		}
	}
	return false;
}

float Ladder::GetMidPosCurrentLadder() const
{
	float p{ (m_Vertices[m_CurrentLadderIndex][0].x
		+ m_Vertices[m_CurrentLadderIndex][2].x) / 2 };
	return p;
}

bool Ladder::GetIsOnTopLadder() const
{
	return m_IsOnTop;
}

