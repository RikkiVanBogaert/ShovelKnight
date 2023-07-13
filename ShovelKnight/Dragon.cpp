#include "pch.h"
#include "Dragon.h"
#include "Gem.h"
#include "Texture.h"

int Dragon::m_NrInstances{};
Texture* Dragon::m_pTexture{};

Dragon::Dragon(const Point2f& pos) :
	Enemy{pos, 7, Vector2f(20, 0)},
	m_DragonState{ DragonState::idle },
	m_TempState{ DragonState::idle },
	m_StateCycleDone{ false },
	m_Position{ 0 },
	m_CurrBubbleIndex{0},
	m_TempAnimFrame{},
	m_Head{0, 0, 60, 20}
{
	++m_NrInstances;
	if (!m_pTexture) m_pTexture = new Texture{ "Resources/Images/DozeDrake.png" };

	m_SrcRect.width = m_pTexture->GetWidth() / 12;
	m_SrcRect.height = m_pTexture->GetHeight() / 5;

	m_Shape.width = m_SrcRect.width;
	m_Shape.height = m_SrcRect.height;

	for (int i{}; i < 6; ++i)
	{
		m_Bubbles.push_back(DragonBubbles{ Point2f(0,0) });
	}
}

Dragon::~Dragon()
{
	--m_NrInstances;
	if (m_NrInstances <= 0)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}


void Dragon::Draw() const
{
	glPushMatrix();
		glTranslatef(m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2, 0);
		glScalef(-1, 1, 1);
		glTranslatef(-m_Shape.left - m_Shape.width / 2, -m_Shape.bottom - m_Shape.height / 2, 0);
		m_pTexture->Draw(m_Shape, m_SrcRect);

	glPopMatrix();
	DrawBubbles();

	//DrawHitBoxes();
}

void Dragon::Update(float elapsedSec)
{
	switch (m_DragonState)
	{
	case DragonState::forward:
		m_Shape.left -= m_Velocity.x * elapsedSec;
		break;
	case DragonState::backward:
		m_Shape.left += m_Velocity.x * elapsedSec;
		break;
	}
	m_Head.left = m_Shape.left + 35;
	m_Head.bottom = m_Shape.bottom;
	if (m_DragonState == DragonState::attack) m_Head.height = 40;
	else m_Head.height = 30;

	ChangeSprites(elapsedSec);
	UpdateState(elapsedSec);
	ActivateBubbles();
	UpdateBubbles(elapsedSec);
	InHit(elapsedSec);
}

void Dragon::UpdateDragonSprites(float elapsedSec, const bool isLooping, const int amountFrames, int fps)
{
	if (m_DragonState != m_TempState)
	{
		m_AnimFrame = 0;
		m_TempState = m_DragonState;
	}

	m_AnimTime += elapsedSec;

	if (m_AnimTime >= 1.0f / fps)
	{
		++m_AnimFrame;
		m_AnimTime = 0;
	}

	if (!isLooping && m_AnimFrame == amountFrames)
	{
		m_StateCycleDone = true;
	}
	else m_AnimFrame %= amountFrames;
}

void Dragon::ChangeSprites(float elapsedSec)
{
	m_SrcRect.left = m_AnimFrame * m_SrcRect.width;
	switch (m_DragonState)
	{
	case DragonState::idle:
		m_SrcRect.bottom = m_SrcRect.height;
		UpdateDragonSprites(elapsedSec, false, 12, 7);
		break;
	case DragonState::forward:
		m_SrcRect.bottom = m_SrcRect.height * 2;
		UpdateDragonSprites(elapsedSec, false, 6, 10);
		break;
	case DragonState::backward:
		m_SrcRect.bottom = m_SrcRect.height * 3;
		UpdateDragonSprites(elapsedSec, false, 6, 10);
		break;
	case DragonState::attack:
		m_SrcRect.bottom = m_SrcRect.height * 4;
		UpdateDragonSprites(elapsedSec, false, 6, 7);
		break;
	case DragonState::poof:
		m_SrcRect.bottom = m_SrcRect.height * 5;
		UpdateDragonSprites(elapsedSec, false, 8, 8);
		break;
	}
}

void Dragon::UpdateState(float elapsedSec)
{
	if (m_StateCycleDone)
	{
		switch (m_DragonState)
		{
		case Dragon::DragonState::idle:
			if(m_Position < 2)
				m_DragonState = DragonState::forward;
			else
				m_DragonState = DragonState::attack;
			break;
		case Dragon::DragonState::forward:
			++m_Position;
			m_DragonState = DragonState::attack;
			break;
		case Dragon::DragonState::backward:
			--m_Position;
			m_DragonState = DragonState::attack;
			break;
		case Dragon::DragonState::attack:
			m_DragonState = DragonState::idle;
			break;
		case Dragon::DragonState::poof:
			m_DragonState = DragonState::dead;
			break;
		}
		m_StateCycleDone = false;
	}
}

void Dragon::GetHit()
{
	--m_Health;
	if (m_Health <= 0) m_DragonState = DragonState::poof;
	else if (m_Position > 0) m_DragonState = DragonState::backward;
}

void Dragon::SpawnGemsDragon(std::vector<Item*>& items)
{
	if (m_DragonState == DragonState::poof)
	{
		m_ActionState = ActionState::poof;
		if (m_AnimFrame % 3 == 0)
		{
			Enemy::SpawnGems(items);
		}
		else m_HasSpawnedGems = false;
	}
}

void Dragon::ActivateBubbles()
{
	if (m_DragonState == DragonState::attack)
	{
		if (m_AnimFrame == 2 && m_TempAnimFrame != m_AnimFrame)
		{
			m_Bubbles[m_CurrBubbleIndex].Activate(Point2f(m_Shape.left + m_Shape.width, m_Shape.bottom));
			IncrementBubbleIndex();
			m_TempAnimFrame = m_AnimFrame;
		}
		if (m_AnimFrame == 4 && m_TempAnimFrame != m_AnimFrame)
		{
			m_Bubbles[m_CurrBubbleIndex].Activate(Point2f(m_Shape.left + m_Shape.width, m_Shape.bottom));
			IncrementBubbleIndex();
			m_TempAnimFrame = m_AnimFrame;
		}
		if (m_AnimFrame == 5 && m_TempAnimFrame != m_AnimFrame)
		{
			m_Bubbles[m_CurrBubbleIndex].Activate(Point2f(m_Shape.left + m_Shape.width, m_Shape.bottom));
			IncrementBubbleIndex();
			m_TempAnimFrame = m_AnimFrame;
		}
	}
}

void Dragon::IncrementBubbleIndex()
{
	if (m_CurrBubbleIndex < m_Bubbles.size() - 1)
		++m_CurrBubbleIndex;
	else m_CurrBubbleIndex = 0;
}

void Dragon::DrawBubbles() const
{
	for (int i{}; i < m_Bubbles.size(); ++i)
	{
		m_Bubbles[i].Draw();
	}
}

void Dragon::UpdateBubbles(float elapsedSec)
{
	for (int i{}; i < m_Bubbles.size(); ++i)
	{
		m_Bubbles[i].Update(elapsedSec);
	}
}

bool Dragon::Collision(const Rectf& actorShape) const
{
	if (m_DragonState != DragonState::poof)
	{
		if (utils::IsOverlapping(actorShape, m_Head) ||
			CheckBodyCollision(actorShape) ||
			BubbleCollision(actorShape))
		{
			return true;
		}
		else return false;
	}
	else return false;
}

bool Dragon::BubbleCollision(const Rectf& actorShape) const
{
	for (int i{}; i < m_Bubbles.size(); ++i)
	{
		if (utils::IsOverlapping(actorShape, m_Bubbles[i].GetShape()) &&
		m_Bubbles[i].GetIsActive())
			return true;
	} 
	return false;
}

bool Dragon::CheckHit(const Rectf& actorHitBox, bool hasKnightAttacked)
{
	if (m_DragonState != DragonState::poof)
	{
		if (utils::IsOverlapping(actorHitBox, m_Head) && !hasKnightAttacked)
		{
			GetHit();
			return true;
		}
		else if (CheckBodyCollision(actorHitBox))
		{
			return true;
		}
		else return false;
	}
	else return false;
}

bool Dragon::CheckHitBubbles(const Rectf& actorHitBox)
{
	if (m_DragonState != DragonState::poof)
	{
		for (int i{}; i < m_Bubbles.size(); ++i)
		{
			if (m_Bubbles[i].GetIsActive())
			{
				if (utils::IsOverlapping(actorHitBox, m_Bubbles[i].GetShape()))
				{
					m_Bubbles[i].IsHit();
					return true;
				}
			}
		}
	}
	return false;
}

bool Dragon::CheckBodyCollision(const Rectf& actorShape) const
{
	Rectf bottom{}, top{};
	bottom.left = m_Shape.left + 85;
	bottom.bottom = m_Shape.bottom;
	bottom.width = 80;
	bottom.height = 57;

	top.left = bottom.left + 7;
	top.bottom = bottom.bottom + bottom.height;
	top.width = 60;
	top.height = 15;

	if (utils::IsOverlapping(actorShape, bottom) ||
		utils::IsOverlapping(actorShape, top))
		return true;
	else
		return false;
}

bool Dragon::IsActive() const
{
	if (m_DragonState != DragonState::poof &&
		m_DragonState != DragonState::dead)
		return true;
	else return false;
}

bool Dragon::GetIsDead() const
{
	if (m_DragonState == DragonState::dead)
		return true;
	else return false;
}

void Dragon::DrawHitBoxes() const
{
	utils::SetColor(Color4f(1, 0, 0, 0.5f));
	utils::FillRect(m_Head);

	Rectf bottom{}, top{};
	bottom.left = m_Shape.left + 85;
	bottom.bottom = m_Shape.bottom;
	bottom.width = 80;
	bottom.height = 57;

	top.left = bottom.left + 7;
	top.bottom = bottom.bottom + bottom.height;
	top.width = 60;
	top.height = 15;

	utils::SetColor(Color4f(0, 0, 1, 0.5f));
	utils::FillRect(bottom);
	utils::SetColor(Color4f(0, 1, 0, 0.5f));
	utils::FillRect(top);
}
