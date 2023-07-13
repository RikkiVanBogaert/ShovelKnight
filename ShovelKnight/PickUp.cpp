#include "pch.h"
#include "PickUp.h"
#include"Texture.h"

int PickUp::m_NrInstances{};
Texture* PickUp::m_pPoofTexture{};

PickUp::PickUp(const Point2f& pos) :
	Item(pos),
	m_State{ State::active }
{
	++m_NrInstances;
	if (!m_pPoofTexture) m_pPoofTexture = new Texture{ "Resources/Images/PickUpSparkle.png" };
}

PickUp::~PickUp()
{
	--m_NrInstances;
	if (m_NrInstances <= 0)
	{
		delete m_pPoofTexture;
		m_pPoofTexture = nullptr;
	}
}

void PickUp::Update(float elapsedSec)
{
	if (m_State == State::poof)
	{
		m_SrcRect.width = m_pPoofTexture->GetWidth() / 3;
		m_SrcRect.height = m_pPoofTexture->GetHeight();
		m_SrcRect.left = m_AnimFrame * m_SrcRect.width;
		m_SrcRect.bottom = m_pPoofTexture->GetHeight();

		m_Shape.width = m_SrcRect.width;
		m_Shape.height = m_SrcRect.height;

		ChangeFrame(elapsedSec, 3, 5, false);
	}
}

void PickUp::IsHit()
{
	if(m_State == State::active)
		m_State = State::poof;
}

void PickUp::ChangeFrame(float elapsedSec, int nrFrames, int fps, bool isLooping)
{
	m_AnimTime += elapsedSec;

	if (m_AnimTime >= 1.0f / fps)
	{
		++m_AnimFrame;
		m_AnimTime = 0;
	}

	if (!isLooping && m_AnimFrame == nrFrames)
	{
		m_State = State::dead;
	}
	else m_AnimFrame %= nrFrames;
}

Rectf PickUp::GetShape() const
{
	return m_Shape;
}

bool PickUp::GetIsDead() const
{
	if (m_State == State::dead)
	{
		return true;
	}
	else return false;
}

bool PickUp::GetIsActive() const
{
	if (m_State == State::active)
	{
		return true;
	}
	else return false;
}

bool PickUp::GetIsPickUp() const
{
	return true;
}
