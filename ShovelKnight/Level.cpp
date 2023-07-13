#include "pch.h"
#include "Level.h"
#include "SVGParser.h"
#include "Waterfall.h"

Level::Level():
	m_FloorTexture{ "Resources/Images/Level/Level1_L1.png" },
	m_CloudsTexture{  "Resources/Images/Level/Clouds.png" },
	m_BG1 { "Resources/Images/Level/BG1.png" },
	m_BG2 { "Resources/Images/Level/BG2adjusted.png" },
	m_BG3 {  "Resources/Images/Level/BG3.png" },
	m_CaveTexture{"Resources/Images/Level/Level1_L3.png"},
	m_CastleTexture{ "Resources/Images/Level/Castle.png" },
	m_pWaterfall{ new Waterfall{} }
{
	SVGParser::GetVerticesFromSvgFile("Resources/Images/Level/Level.svg", m_Vertices);
	SVGParser::GetVerticesFromSvgFile("Resources/Images/Level/Spikes.svg", m_SpikeVertices);
	m_Boundaries = Rectf(0, 0, m_FloorTexture.GetWidth(), m_FloorTexture.GetHeight());
}

Level::~Level()
{
	for (size_t i{}; i < m_Vertices.size(); ++i)
	{
		m_Vertices.pop_back();
	}
	for (size_t i{}; i < m_SpikeVertices.size(); ++i)
	{
		m_SpikeVertices.pop_back();
	}
	delete m_pWaterfall;
}


void Level::Draw(const Vector2f& transform, float scale, const Point2f& playerPos) const
{
	//for parallex
	float parallexEffect{};

	glPushMatrix();
		parallexEffect = 0.1f;
		glTranslatef(transform.x * parallexEffect, transform.y, 0);
		glScalef(scale, scale, 1);
		m_CastleTexture.Draw();
	glPopMatrix();

	glPushMatrix();
	parallexEffect = 0.4f;
		glTranslatef(transform.x * parallexEffect, transform.y, 0);
		glScalef(scale, scale, 1);
		m_BG3.Draw();
	glPopMatrix();
	
	glPushMatrix();
	parallexEffect = 0.6f;
		glTranslatef(transform.x * parallexEffect, transform.y, 0);
		glScalef(scale, scale, 1);
		m_BG2.Draw();
	glPopMatrix();
	
	glPushMatrix();
	parallexEffect = 0.8f;
		glTranslatef(transform.x * parallexEffect, transform.y , 0);
		glScalef(scale, scale, 1);
		m_BG1.Draw();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(transform.x, transform.y, 0);
		glScalef(scale, scale, 1);
		m_pWaterfall->Draw(playerPos);
		m_CaveTexture.Draw();
		m_FloorTexture.Draw();
	glPopMatrix();
}

void Level::DrawClouds(const Rectf& window) const
{
	m_CloudsTexture.Draw(window);
}

void Level::Update(float elapsedSec)
{
	m_pWaterfall->Update(elapsedSec);
}

void Level::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, bool isNPC)
{
	Point2f top{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height };
	Point2f bottom{ actorShape.left + actorShape.width / 2, actorShape.bottom };

	Point2f left{ actorShape.left , actorShape.bottom + actorShape.height / 3 };
	Point2f right{ actorShape.left + actorShape.width , actorShape.bottom + actorShape.height / 3 };

	Point2f mid{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height * 0.8f };

	utils::HitInfo hitInfo1{};

	for (int i{}; i < m_Vertices.size(); ++i)
	{
		if (utils::Raycast(m_Vertices[i], mid, bottom, hitInfo1))
		{
			actorShape.bottom = hitInfo1.intersectPoint.y;
			actorVelocity.y = 0;

		}

		if (utils::Raycast(m_Vertices[i], mid, top, hitInfo1))
		{
			actorShape.bottom = hitInfo1.intersectPoint.y - actorShape.height;
			actorVelocity.y = -1;
		}


		//sides
		if (utils::Raycast(m_Vertices[i], mid, right, hitInfo1))
		{
			actorShape.left = hitInfo1.intersectPoint.x - actorShape.width;
			if (!isNPC) actorVelocity.x = 0;
			else actorVelocity.x *= -1;
		}
		if (utils::Raycast(m_Vertices[i], mid, left, hitInfo1))
		{
			actorShape.left = hitInfo1.intersectPoint.x;
			if (!isNPC) actorVelocity.x = 0;
			else actorVelocity.x *= -1;
		}
	}

	CheckOutOfBounds(actorShape, actorVelocity, isNPC);
}

void Level::CheckOutOfBounds(Rectf& actorShape, Vector2f& actorVelocity, const bool isNPC)
{
	if (actorShape.left + actorShape.width >= m_Boundaries.left + m_Boundaries.width)
	{
		actorShape.left = m_Boundaries.left + m_Boundaries.width - actorShape.width;
		if (!isNPC) actorVelocity.x = 0;
		else actorVelocity.x *= -1;
	}
	if (actorShape.left < m_Boundaries.left)
	{
		actorShape.left = m_Boundaries.left;
		if (!isNPC) actorVelocity.x = 0;
		else actorVelocity.x *= -1;
	}
}

bool Level::IsOnGround(const Rectf& actorShape) const
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
	return false;
}

bool Level::IsInSpikes(const Rectf& actorShape)
{
	Point2f mid{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 2 };

	for (int i{}; i < m_SpikeVertices.size(); ++i)
	{
		if (utils::IsPointInPolygon(mid, m_SpikeVertices[i]))
		{
			return true;
		}
	}
	return false;
}

Rectf Level::GetBoundaries() const
{
	return m_Boundaries;
}
