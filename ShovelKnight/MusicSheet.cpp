#include "pch.h"
#include "MusicSheet.h"
#include "Texture.h"

int MusicSheet::m_NrInstances{};
Texture* MusicSheet::m_pTexture{};

MusicSheet::MusicSheet(const Point2f& pos):
	PickUp(pos),
	m_MinY{pos.y - 4},
	m_MaxY{pos.y + 4},
	m_IsGoingUp{true}
{
	++m_NrInstances;
	if (!m_pTexture) m_pTexture = new Texture{ "Resources/Images/MusicSheet.png" };

	m_SrcRect.width = m_pTexture->GetWidth();
	m_SrcRect.height = m_pTexture->GetHeight();
	m_Shape.width = m_SrcRect.width;
	m_Shape.height = m_SrcRect.height;
}

MusicSheet::~MusicSheet()
{
	--m_NrInstances;
	if (m_NrInstances <= 0)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

void MusicSheet::Draw() const
{
	if (m_State == State::active)
	{
		m_pTexture->Draw(m_Shape, m_SrcRect);
	}
	else if (m_State == State::poof)
	{
		m_pPoofTexture->Draw(m_Shape, m_SrcRect);
	}
}

void MusicSheet::Update(float elapsedSec)
{
	if (m_State == State::active)
	{
		MoveUpDown(elapsedSec);
	}
	PickUp::Update(elapsedSec);
}

void MusicSheet::MoveUpDown(float elapsedSec)
{
	const float midYPos{ m_Shape.bottom + m_Shape.height / 2 };
	const float speed{ 5 * elapsedSec };

	if(m_IsGoingUp) m_Shape.bottom += speed;
	else m_Shape.bottom -= speed;

	if (midYPos >= m_MaxY && m_IsGoingUp)
		m_IsGoingUp = false;
	else if (midYPos <= m_MinY && !m_IsGoingUp)
		m_IsGoingUp = true;
}