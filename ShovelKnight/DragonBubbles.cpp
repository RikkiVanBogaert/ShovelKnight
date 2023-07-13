#include "pch.h"
#include "DragonBubbles.h"

DragonBubbles::DragonBubbles(const Point2f& pos):
	m_BeginPos{pos},
    m_Shape{pos.y, pos.x, 0, 0},
	m_SrcRect{},
	m_BubbleTexture{ "Resources/Images/Bubble.png" },
	m_BubblePopTexture{ "Resources/Images/BubblePop.png" },
	m_State{ State::active },
	m_Speed{ 170, 50},
	m_AnimTime{0},
	m_AnimFrame{},
	m_ActionState{ActionState::inactive},
	m_MaxX{rand() % 30 + 50}
{
	m_SrcRect.width = m_BubbleTexture.GetWidth() / 4;
	m_SrcRect.height = m_BubbleTexture.GetHeight();

	m_Shape.width = m_SrcRect.width;
	m_Shape.height = m_SrcRect.height;
}

void DragonBubbles::Draw() const
{
	if (m_State == State::active)
		m_BubbleTexture.Draw(m_Shape, m_SrcRect);
	else if (m_State == State::popped)
		m_BubblePopTexture.Draw(m_Shape, m_SrcRect);

	//DrawHitBox();
}

void DragonBubbles::Update(float elapsedSec)
{
	if (m_State == State::active)
	{
		Movement(elapsedSec);

		ChangeFrame(elapsedSec, 4, 8, true);
	}
	else if (m_State == State::popped)
	{
		m_SrcRect.width = m_BubblePopTexture.GetWidth() / 2;
		m_SrcRect.height = m_BubblePopTexture.GetHeight();

		ChangeFrame(elapsedSec, 2, 5, false);
	}
	else if (m_State == State::dead)
	{
		m_State = State::active;
		m_Shape.left = 0;
	}

	m_SrcRect.left = m_SrcRect.width * m_AnimFrame;
}

void DragonBubbles::ChangeFrame(float elapsedSec, int nrFrames, int fps, bool isLooping)
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

void DragonBubbles::Activate(const Point2f& pos)
{
	const Point2f offset{180, 15};
	if (m_ActionState == ActionState::inactive)
	{
		m_ActionState = ActionState::horizontal;
		m_BeginPos.x = pos.x - offset.x;
		m_BeginPos.y = pos.y + offset.y;
		m_Shape.left = m_BeginPos.x;
		m_Shape.bottom = m_BeginPos.y;
	}
	
}

void DragonBubbles::Movement(float elapsedSec)
{
	if (m_ActionState == ActionState::horizontal)
	{
		if (m_Shape.left < m_BeginPos.x - m_MaxX)
			m_ActionState = ActionState::vertical;
		else
			m_Shape.left -= m_Speed.x * elapsedSec;
	}
	if (m_ActionState == ActionState::vertical)
	{
		m_Shape.bottom += m_Speed.y * elapsedSec;
	}

	if (m_Shape.bottom > 1020)
	{
		m_ActionState = ActionState::inactive;
		m_Shape.left = 0;
		m_Shape.bottom = 0;
	}
}

void DragonBubbles::IsHit()
{
	if (m_State == State::active)
	{
		m_AnimFrame = 0;
		m_State = State::popped;
	}
}

Rectf DragonBubbles::GetShape() const
{
	const float offset{ 6 };
	Rectf hurtBox{ m_Shape.left + offset, m_Shape.bottom + offset,
		m_Shape.width - 2 * offset, m_Shape.height -  2 * offset };
	return hurtBox;
}

bool DragonBubbles::GetIsDead() const
{
	return false;
}

bool DragonBubbles::GetIsActive() const
{
	if (m_State == State::active) return true;
	else return false;
}

void DragonBubbles::DrawHitBox() const
{
	const float offset{ 6 };
	Rectf hurtBox{ m_Shape.left + offset, m_Shape.bottom + offset,
		m_Shape.width - 2 * offset, m_Shape.height - 2 * offset };
	utils::SetColor(Color4f(1, 0, 0, 0.4f));
	utils::FillRect(hurtBox);
}