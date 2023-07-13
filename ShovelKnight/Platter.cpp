#include "pch.h"
#include "Platter.h"
#include "Texture.h"

int Platter::m_NrInstances{};
Texture* Platter::m_pClosedTexture{}, *Platter::m_pOpenTexture{};

Platter::Platter(const Point2f& pos):
	Interactable(pos),
	m_DeletePartsTimer{},
	m_Velocity{90, 150}
{
	++m_NrInstances;
	if (!m_pClosedTexture) m_pClosedTexture = new Texture{ "Resources/Images/Platter.png" };
	if (!m_pOpenTexture) m_pOpenTexture = new Texture{ "Resources/Images/PlatterParts.png" };

	m_Shape.width = m_pClosedTexture->GetWidth();
	m_Shape.height = m_pClosedTexture->GetHeight();

	m_BottomPart = m_Shape;
}

Platter::~Platter()
{
	--m_NrInstances;
	if (m_NrInstances <= 0)
	{
		delete m_pClosedTexture;
		m_pClosedTexture = nullptr;
		delete m_pOpenTexture;
		m_pOpenTexture = nullptr;
	}
}

void Platter::Draw() const
{
	if (m_State == State::active)
		m_pClosedTexture->Draw(m_Shape);
	else if (m_State == State::inactive)
	{
		const Rectf srcRectTop{0, m_pOpenTexture->GetHeight() / 2, 
			m_pOpenTexture->GetWidth(), m_pOpenTexture->GetHeight() / 2};
		const Rectf srcRectBottom{ srcRectTop.left, m_pOpenTexture->GetHeight(), 
			srcRectTop.width, srcRectTop.height };

		m_pOpenTexture->Draw(m_Shape, srcRectTop);
		m_pOpenTexture->Draw(m_BottomPart, srcRectBottom);
	}
}

void Platter::Update(float elapsedSec)
{
	if (m_State == State::inactive)
	{
		m_Shape.width = m_pOpenTexture->GetWidth();
		m_Shape.height = m_pOpenTexture->GetHeight() / 2;
		m_BottomPart.width = m_Shape.width;
		m_BottomPart.height = m_Shape.height;

		m_DeletePartsTimer += elapsedSec;
		if (m_DeletePartsTimer >= 1.f)
		{
			m_State = State::dead;
		}

		const float gravity{ -1000 };

		m_Velocity.y += gravity * elapsedSec;

		m_Shape.left += m_Velocity.x * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;

		m_BottomPart.left -= m_Velocity.x * elapsedSec;
		m_BottomPart.bottom += m_Velocity.y * elapsedSec;
	}
}
