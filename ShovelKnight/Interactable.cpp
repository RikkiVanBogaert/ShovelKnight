#include "pch.h"
#include "Interactable.h"

Interactable::Interactable(const Point2f& pos):
	Item(pos),
	m_State{State::active}
{}

Interactable::~Interactable()
{}


void Interactable::IsHit()
{
	if(m_State == State::active)
		m_State = State::inactive;
}

void Interactable::SpawnGems(std::vector<Item*>&items, bool isKnightRight)
{}

void Interactable::ChangeFrame(float elapsedSec, int nrFrames, int fps, bool isLooping)
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

Rectf Interactable::GetShape() const
{
	return m_Shape;
}

bool Interactable::GetIsDead() const
{
	if (m_State == State::dead)
	{
		return true;
	}
	else return false;
}

bool Interactable::GetIsActive() const
{
	if (m_State == State::active)
	{
		return true;
	}
	else return false;
}

bool Interactable::GetIsPickUp() const
{
	return false;
}