#include "pch.h"
#include "Checkpoint.h"
#include "Texture.h"

int Checkpoint::m_NrInstances{};
Texture* Checkpoint::m_pTexture{nullptr};

Checkpoint::Checkpoint(const Point2f& pos) :
	PickUp{pos},
	m_State{ State::inactive },
	m_SrcRectBall{}
{
	++m_NrInstances;
	if (!m_pTexture) m_pTexture = new Texture{ "Resources/Images/CheckPoint.png" };

	m_SrcRect.left = 0;
	m_SrcRect.bottom = m_pTexture->GetHeight();
	m_SrcRect.width = 38;
	m_SrcRect.height = 56;

	m_SrcRectBall.width = 23;
	m_SrcRectBall.height = 22;

	m_Shape.width = m_SrcRect.width;
	m_Shape.height = m_SrcRect.height;
}

Checkpoint::~Checkpoint()
{
	--m_NrInstances;
	if (m_NrInstances <= 0)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

void Checkpoint::Draw() const
{
	Rectf ballDestRect{0, m_Shape.bottom + 30, m_SrcRectBall.width, m_SrcRectBall.height};
	ballDestRect.left = m_Shape.left + 7;
	if(m_State != State::inactive) m_pTexture->Draw(ballDestRect, m_SrcRectBall);
	m_pTexture->Draw(m_Shape, m_SrcRect);
}

void Checkpoint::Update(float elapsedSec)
{
	UpdateSprites(elapsedSec);
}

void Checkpoint::UpdateSprites(float elapsedSec)
{
	if (m_State == State::activated)
	{
		m_SrcRectBall.left = m_AnimFrame * m_SrcRectBall.width;
		m_SrcRectBall.bottom = 42;
		ChangeFrame(elapsedSec, 5, 10, false);
	}
	if (m_State == State::active)
	{
		m_SrcRectBall.left = m_AnimFrame * m_SrcRectBall.width;
		m_SrcRectBall.bottom = 21;
		ChangeFrame(elapsedSec, 3, 7, true);
	}
}

void Checkpoint::ChangeFrame(float elapsedSec, int nrFrames, int fps, bool isLooping)
{
	m_AnimTime += elapsedSec;

	if (m_AnimTime >= 1.0f / fps)
	{
		++m_AnimFrame;
		m_AnimTime = 0;
	}

	if (!isLooping && m_AnimFrame == nrFrames)
	{
		m_State = State::active;
	}
	else m_AnimFrame %= nrFrames;
}

void Checkpoint::IsHit()
{
	if (m_State == State::inactive)
		m_State = State::activated;
}

bool Checkpoint::GetIsDead() const
{
	return false;
}

bool Checkpoint::GetIsActive() const
{
	if (m_State == State::inactive)
		return true;
	else return false;
}