#include "pch.h"
#include "DirtPile.h"
#include "Texture.h"
#include "Gem.h"

int DirtPile::m_NrInstances{};
Texture* DirtPile::m_pDirtPileTexture{};

DirtPile::DirtPile(const Point2f& pos, float angle) :
	Interactable(pos),
	m_SizePile{5},
	m_Angle{angle}
{
	++m_NrInstances;
	if (!m_pDirtPileTexture) m_pDirtPileTexture = new Texture{ "Resources/Images/DirtPile.png" };

	m_SrcRect.width = m_pDirtPileTexture->GetWidth() / 5;
	m_SrcRect.height = m_pDirtPileTexture->GetHeight() / 5;
	m_SrcRect.bottom = m_SrcRect.height;

	m_Shape.width = m_SrcRect.width;
	m_Shape.height = m_SrcRect.height;
}

DirtPile::~DirtPile()
{
	--m_NrInstances;
	if (m_NrInstances <= 0)
	{
		delete m_pDirtPileTexture;
		m_pDirtPileTexture = nullptr;
	}
}

void DirtPile::Draw() const
{
	glPushMatrix();
		glTranslatef(m_Shape.left + m_Shape.width, m_Shape.bottom + m_Shape.height, 0);
		glRotatef(m_Angle, 0, 0, 1);
		glTranslatef(-m_Shape.left - m_Shape.width, -m_Shape.bottom - m_Shape.height, 0);
		m_pDirtPileTexture->Draw(m_Shape, m_SrcRect);
	glPopMatrix();
}

void DirtPile::Update(float elapsedSec)
{}

void DirtPile::IsHit()
{
	--m_SizePile;
	m_SrcRect.bottom += m_SrcRect.height;
}

void DirtPile::SpawnGems(std::vector<Item*>& items, bool isKnightRight)
{
	const int amount{ rand() % 5 + 3 };
	Point2f spawnPos{ };
	spawnPos.x = m_Shape.left + m_Shape.width / 2;
	spawnPos.y = m_Shape.bottom + m_Shape.height;

	for (int i{}; i < amount; ++i)
	{
		Vector2f velocity{ float(rand() % 75 + 50), float(rand() % 50 + 80) };
		if(isKnightRight) velocity.x *= -1;
		items.push_back(new Gem{ spawnPos, velocity });
	}
}

Rectf DirtPile::GetShape() const
{
	Rectf shape{ m_Shape };
	if (m_Angle == 90)
	{
		shape.left += m_SrcRect.width;
		shape.bottom -= m_SrcRect.width / 2;
		shape.width = m_SrcRect.height;
		shape.height = m_SrcRect.width;
	}
	return shape;
}

bool DirtPile::GetIsDead() const
{
	if (m_SizePile <= 0)
	{
		return true;
	}
	else return false;
}

bool DirtPile::GetIsActive() const
{
	if (m_SizePile > 0)
	{
		return true;
	}
	else return false;
}
