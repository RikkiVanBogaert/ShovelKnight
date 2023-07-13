#include "pch.h"
#include "Gem.h"
#include "Texture.h"
#include "SoundEffect.h"

int Gem::m_NrInstances{};
Texture* Gem::m_pGemTexture{ nullptr };
SoundEffect* Gem::m_pCollected{ nullptr };

Gem::Gem(const Point2f& pos, const Vector2f& velocity, bool isBigDiamond ) :
	PickUp(pos),
	m_Velocity{velocity},
	m_IsOnGround{true}
{
	++m_NrInstances;
	if (!m_pGemTexture) m_pGemTexture = new Texture{ "Resources/Images/Gem.png" };
	if (!m_pCollected) m_pCollected = new SoundEffect{ "Resources/Sounds/GemStandard.wav" };
	m_SrcRect.bottom = 0;
	m_SrcRect.width = m_pGemTexture->GetWidth() / 6;
	m_SrcRect.height = m_pGemTexture->GetHeight();
	if(!isBigDiamond) GetRandomGem();
	else
	{
		m_SrcRect.left = m_SrcRect.width * 5;
		m_Value = 30;
	}
	m_Shape.width = m_SrcRect.width;
	m_Shape.height = m_SrcRect.height;

	if (m_Velocity.y != 0)
	{
		m_State = State::spawning;
		m_IsOnGround = false;
	}

	m_Shape.left = pos.x - m_Shape.width / 2;
	m_Shape.bottom = pos.y - m_Shape.height / 2;
}

void Gem::GetRandomGem()
{
	int rndNr{ rand() % 30 };
	if (rndNr < 10)
	{
		m_SrcRect.left = 0;
		m_Value = 1;
	}
	else if (rndNr < 18)
	{
		m_SrcRect.left = m_SrcRect.width;
		m_Value = 2;
	}
	else if (rndNr < 24)
	{
		m_SrcRect.left = m_SrcRect.width * 2;
		m_Value = 5;
	}
	else if (rndNr < 28)
	{
		m_SrcRect.left = m_SrcRect.width * 3;
		m_Value = 10;
	}
	else
	{
		m_SrcRect.left = m_SrcRect.width * 4;
		m_Value = 15;
	}
}

Gem::~Gem()
{
	--m_NrInstances;
	if (m_NrInstances <= 0)
	{
		delete m_pGemTexture;
		m_pGemTexture = nullptr;

		delete m_pCollected;
		m_pCollected = nullptr;
	}
}

void Gem::Draw() const
{
	if (m_State == State::active || m_State == State::spawning)
	{
		m_pGemTexture->Draw(m_Shape, m_SrcRect);
	}
	else if (m_State == State::poof)
	{
		m_pPoofTexture->Draw(m_Shape, m_SrcRect);
	}
}

void Gem::Update(float elapsedSec)
{
	PickUp::Update(elapsedSec);
	LilJump(elapsedSec);
}

void Gem::LilJump( float elapsedSec)
{
	if (m_Velocity.y != 0 && !m_IsOnGround)
	{
		const float gravity{ -1000 };

		m_Velocity.y += gravity * elapsedSec;
		m_Shape.left += m_Velocity.x * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;
	}
	else if (m_IsOnGround && m_State == State::spawning) m_State = State::active;
}

void Gem::IsHit()
{
	if (m_State == State::active)
	{
		m_State = State::poof;
		m_pCollected->Play(false);
	}
}

void Gem::SetIsOnGround(bool isOnGround)
{
	m_IsOnGround = isOnGround;
}

void Gem::SetVolume(int volume)
{
	m_pCollected->SetVolume(volume);
}

int Gem::GetValue() const
{
	return m_Value;
}

Rectf& Gem::GetLevelCollisionShape()
{
	return m_Shape;
}

Vector2f& Gem::GetVelocity()
{
	return m_Velocity;
}


void Gem::DrawHurtBox() const
{
	const float offset{ 4 };
	Rectf box{};
	box.left = m_Shape.left + offset;
	box.bottom = m_Shape.bottom + offset;
	box.width = m_Shape.width - offset * 2;
	box.height = m_Shape.height - offset * 2;

	utils::SetColor(Color4f(1, 0, 0, 0.5f));
	utils::FillRect(box);
}