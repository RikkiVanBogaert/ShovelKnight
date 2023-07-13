#include "pch.h"
#include "Bubble.h"
#include "Texture.h"

int Bubble::m_NrInstances{};
Texture* Bubble::m_pBubbleTexture{ nullptr };
Texture* Bubble::m_pBubblePopTexture{ nullptr };

Bubble::Bubble(const Point2f& pos, float maxY):
	Interactable(pos),
	m_UpSpeed{30},
	m_BeginY{pos.y},
	m_MaxY{maxY}
{
	++m_NrInstances;
	if (!m_pBubbleTexture) m_pBubbleTexture = new Texture{ "Resources/Images/Bubble.png" };
	if (!m_pBubblePopTexture) m_pBubblePopTexture = new Texture{ "Resources/Images/BubblePop.png" };

	m_Shape.width = m_pBubbleTexture->GetWidth() / 4;
	m_Shape.height = m_pBubbleTexture->GetHeight();
}

Bubble::~Bubble()
{
	--m_NrInstances;

	if (m_NrInstances <= 0)
	{
		delete m_pBubbleTexture;
		m_pBubbleTexture = nullptr;
		delete m_pBubblePopTexture;
		m_pBubblePopTexture = nullptr;
	}
}

void Bubble::Draw() const
{
	if (m_State == State::active) 
		m_pBubbleTexture->Draw(m_Shape, m_SrcRect);
	else if (m_State == State::inactive) 
		m_pBubblePopTexture->Draw(m_Shape, m_SrcRect);
}

void Bubble::Update(float elapsedSec)
{
	Point2f clipSize{};

	if (m_State == State::active)
	{
		clipSize.x = m_pBubbleTexture->GetWidth() / 4;
		clipSize.y = m_pBubbleTexture->GetHeight();

		if(m_MaxY > m_Shape.bottom) m_Shape.bottom += m_UpSpeed * elapsedSec;
		ChangeFrame(elapsedSec, 4, 8, true);
	}
	else if (m_State == State::inactive)
	{
		clipSize.x = m_pBubblePopTexture->GetWidth() / 2;
		clipSize.y = m_pBubblePopTexture->GetHeight();

		ChangeFrame(elapsedSec, 2, 5, false);
	}

	m_SrcRect.left = clipSize.x * m_AnimFrame;
	m_SrcRect.bottom = clipSize.y;
	m_SrcRect.width = clipSize.x;
	m_SrcRect.height = clipSize.y;

	Respawn();
}

void Bubble::Respawn()
{
	if (m_State == State::dead)
	{
		m_Shape.bottom = m_BeginY;
		m_State = State::active;
	}
}