#include "pch.h"
#include "Chest.h"
#include "Gem.h"
#include "Texture.h"

int Chest::m_NrInstances{};
Texture* Chest::m_pChestTexture{ nullptr };

Chest::Chest(const Point2f& pos, bool isFacingLeft) :
	Item(pos),
	m_State{State::closed},
	m_LootTime{2},
	m_LootTimer{},
	m_IsFacingLeft{isFacingLeft}
{
	++m_NrInstances;
	if (!m_pChestTexture) m_pChestTexture = new Texture{ "Resources/Images/TreasureChest.png" };

	m_SrcRect.width = m_pChestTexture->GetWidth();
	m_SrcRect.height = m_pChestTexture->GetHeight() / 2;
	m_SrcRect.bottom = m_SrcRect.height;

	m_Shape.width = m_SrcRect.width;
	m_Shape.height = m_SrcRect.height;
}

Chest::~Chest()
{
	--m_NrInstances;

	if (m_NrInstances <= 0)
	{
		delete m_pChestTexture;
		m_pChestTexture = nullptr;
	}
}

void Chest::Draw() const
{
	glPushMatrix();
	if (!m_IsFacingLeft)
	{
		glTranslatef(m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2, 0);
		glScalef(-1, 1, 1);
		glTranslatef(-m_Shape.left - m_Shape.width / 2, -m_Shape.bottom - m_Shape.height / 2, 0);
	}
	m_pChestTexture->Draw(m_Shape, m_SrcRect);
	glPopMatrix();
}

void Chest::Update(float elapsedSec)
{
	if (m_State == State::open)
	{
		m_LootTimer += elapsedSec;
		if (m_LootTimer >= m_LootTime)
			m_State = State::empty;
	}
	UpdateSprite();
}

void Chest::UpdateSprite()
{
	if (m_State == State::closed)
	{
		m_SrcRect.bottom = m_SrcRect.height;
	}
	else if (m_State == State::open)
	{
		m_SrcRect.bottom = m_pChestTexture->GetHeight();
	}
}

void Chest::IsHit()
{
	if (m_State == State::closed)
		m_State = State::open;
}

void Chest::SpawnGems(std::vector<Item*>& items, bool isKnightRight)
{
	const Point2f spawnPos{ m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2 };
	if ((int(m_LootTimer * 100) % 20) <= 3)
	{
		Vector2f velocity{ float(rand() % 50 + 130), float(rand() % 50 + 140) };
		int rndNr{ rand() % 2 };
		if (rndNr == 0) velocity.x *= -1;
		items.push_back(new Gem{ spawnPos, velocity });
	}
	
}

bool Chest::GetIsDead() const
{
	return false;
}

bool Chest::GetIsActive() const
{
	if (m_State != State::empty)
	{
		return true;
	}
	else return false;
}

bool Chest::GetIsOpen() const
{
	if (m_State == State::open) return true;
	else return false;
}

bool Chest::GetIsFacingLeft() const
{
	return m_IsFacingLeft;
}