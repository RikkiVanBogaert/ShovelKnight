#include "pch.h"
#include "Knight.h"
#include "Texture.h"
#include "Vector2f.h"

Knight::Knight(const Point2f& pos) :
	m_pSpriteSheet{ new Texture{"Resources/Images/ShovelKnightTrans.png"} },
	m_Shape{ pos.x, pos.y, 30, 26 },
	m_ActionState{ ActionState::idle },
	m_Health{8},
	m_Velocity{ 0, 0 },
	m_HorSpeed{ 110 },
	m_Gravity{ -1200 },
	m_JumpSpeed{ 380 },
	m_RespawnPos{pos},
	m_IsGoingRight{ true },
	m_IsOnGround{ true },
	m_StunTimer{ 2 },
	m_IsInvincible{ false },
	m_IsOnLadder{ false },
	m_IsOnTopOfLadder{false},
	m_HasAttacked{false},
	m_HasAppliedKnockBack{false},
	m_IsJumpPressed{ false },
	m_IsAttackPressed{false},
	m_NrFramesPerSec{ 14 },
	m_AnimTime{ 1 },
	m_AnimFrame{},
	m_TempActionState{},
	m_pJump{ new SoundEffect{"Resources/Sounds/Hero_Jump.wav"} },
	m_pLand{ new SoundEffect{"Resources/Sounds/Hero_Land.wav"} },
	m_pDig{ new SoundEffect{"Resources/Sounds/Hero_Dig.wav"} },
	m_pBounce{ new SoundEffect{"Resources/Sounds/Hero_Bounce.wav"} }
{
	m_SrcRect.width = m_pSpriteSheet->GetWidth() / 9;
	m_SrcRect.height = m_pSpriteSheet->GetHeight() / 9;
}

Knight::~Knight()
{
	delete m_pSpriteSheet;
	m_pSpriteSheet = nullptr;
	delete m_pJump;
	delete m_pLand;
	delete m_pDig;
	delete m_pBounce;
}

void Knight::Draw() const
{

	const float scale{ 2.1f };
	Rectf destRect{ m_Shape.left - 5, m_Shape.bottom - 5, m_Shape.width * scale, m_Shape.height * scale };

	glPushMatrix();
	if (!m_IsGoingRight)
	{
		glTranslatef(m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2, 0);
		glScalef(-1, 1, 1);
		glTranslatef(-m_Shape.left - m_Shape.width / 2, -m_Shape.bottom - m_Shape.height / 2, 0);
	}

	if (m_StunTimer < 1.8f && m_ActionState != ActionState::dying) // flicker when hit
	{
		int interval{ 2 };
		if (int(m_StunTimer * 10) % interval == 0)
		{
			m_pSpriteSheet->Draw(destRect, m_SrcRect);
		}

	}
	else m_pSpriteSheet->Draw(destRect, m_SrcRect);

	glPopMatrix();

	//TESTING

	//utils::SetColor(Color4f(0, 1, 0, 0.4f)); //draw hurtbox
	//utils::FillRect(m_Shape);
}

void Knight::ChangeSpriteCoordinates(float elapsedSec)
{

	switch (m_ActionState)
	{
	case ActionState::idle:
		m_SrcRect.left = 0;
		m_SrcRect.bottom = m_SrcRect.height;
		break;
	case ActionState::moving:
		m_SrcRect.left = m_AnimFrame * m_SrcRect.width;
		m_SrcRect.bottom = 2 * m_SrcRect.height;
		ChangeFrame(elapsedSec, 6, 10, true);
		break;
	case ActionState::jumping:
		if (m_Velocity.y > 0)
		{
			m_SrcRect.left = 0;
			m_SrcRect.bottom = 4 * m_SrcRect.height;
		}
		else
		{
			m_SrcRect.left = m_SrcRect.width;
			m_SrcRect.bottom =  4 * m_SrcRect.height;
		}
		break;
	case ActionState::jumpAttack:
		m_SrcRect.left = 2 * m_SrcRect.width;
		m_SrcRect.bottom = 4 * m_SrcRect.height;
		break;
	case ActionState::attack:
		m_SrcRect.left = m_AnimFrame * m_SrcRect.width;
		m_SrcRect.bottom = 5 * m_SrcRect.height;
		ChangeFrame(elapsedSec, 5, 14, false);
		break;
	case ActionState::getHit:
		m_SrcRect.left = 0;
		m_SrcRect.bottom = 3 * m_SrcRect.height;
		break;
	case ActionState::climbing:
		m_SrcRect.left = m_SrcRect.width * 4;
		m_SrcRect.bottom = 6 * m_SrcRect.height;
		break;
	case ActionState::looting:
		m_SrcRect.left = m_AnimFrame * m_SrcRect.width;
		m_SrcRect.bottom = 8 * m_SrcRect.height;
		ChangeFrame(elapsedSec, 2, 8, true);
		break;
	case ActionState::finishLooting:
		m_SrcRect.left = m_SrcRect.width * (m_AnimFrame + 2);
		m_SrcRect.bottom = 8 * m_SrcRect.height;
		ChangeFrame(elapsedSec, 2, 2, false);
		break;
	case ActionState::dying:
		m_SrcRect.left = m_AnimFrame * m_SrcRect.width;
		m_SrcRect.bottom = 7 * m_SrcRect.height;
		ChangeFrame(elapsedSec, 3, 1, false);
		break;
	case ActionState::dead:
		m_SrcRect.left = 2 * m_SrcRect.width;
		m_SrcRect.bottom = 7 * m_SrcRect.height;
		break;
	}
}

void Knight::ChangeFrame(float elapsedSec, int nrFrames, int fps, bool isLooping)
{
	ResetAnimFrame();

	m_AnimTime += elapsedSec;
	
	if (m_AnimTime >= 1.0f / fps)
	{
		++m_AnimFrame;
		m_AnimTime = 0;
	}
	
	if (!isLooping && m_AnimFrame == nrFrames)
	{
		if (m_ActionState != ActionState::dying)
		{
			m_AnimFrame = 0;
			m_ActionState = ActionState::idle;
		}
		else m_ActionState = ActionState::dead;
	}
	else m_AnimFrame %= nrFrames;
}

void Knight::ResetAnimFrame()
{
	if (m_ActionState != m_TempActionState)
		m_AnimFrame = 0;

	m_TempActionState = m_ActionState;
}

void Knight::Update(float elapsedSec)
{
	if (m_ActionState != ActionState::attack && m_ActionState != ActionState::getHit &&
		m_ActionState != ActionState::looting && m_ActionState != ActionState::finishLooting &&
		m_ActionState != ActionState::dying && m_ActionState != ActionState::dead)
	{
		HandleKeyInputs(elapsedSec);
		Jump(elapsedSec);
	}

	Attack();
	KnockBack(elapsedSec);

	if (m_ActionState != ActionState::climbing)
	{
		m_Velocity.y += m_Gravity * elapsedSec;
		m_Shape.left += m_Velocity.x * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;
	}

	ChangeSpriteCoordinates(elapsedSec);

}

void Knight::HandleKeyInputs(float elapsedSec)
{
	const Uint8* pStates{ SDL_GetKeyboardState(nullptr) };

	if (pStates[SDL_SCANCODE_A])
	{
		if(m_IsOnGround) m_ActionState = ActionState::moving;
		m_Velocity.x = -m_HorSpeed;
		m_IsGoingRight = false;
	}
	else if (pStates[SDL_SCANCODE_D])
	{
		if (m_IsOnGround) m_ActionState = ActionState::moving;
		m_Velocity.x = m_HorSpeed;
		m_IsGoingRight = true;
	}
	else
	{
		if ((m_ActionState != ActionState::climbing || m_IsOnLadder ) && m_IsOnGround)
			m_ActionState = ActionState::idle;

		if (m_Velocity.x == m_HorSpeed ||
			m_Velocity.x == -m_HorSpeed) //for checking if hes not moving at the speed of a platform
			m_Velocity.x = 0;
	}

	//charge jump
	if ((pStates[SDL_SCANCODE_SPACE] || pStates[SDL_SCANCODE_K]) && m_IsOnGround)
	{
		if (!m_IsJumpPressed) m_pJump->Play(false);
		m_IsJumpPressed = true;
	}
	if ((pStates[SDL_SCANCODE_SPACE] || pStates[SDL_SCANCODE_K]) &&
		m_JumpSpeed > 130 && m_IsJumpPressed)
	{
		m_Velocity.y = m_JumpSpeed;
		m_JumpSpeed -= 490 * elapsedSec;
	}
	else if (!pStates[SDL_SCANCODE_SPACE] && !pStates[SDL_SCANCODE_K])
	{
		m_IsJumpPressed = false;
		m_JumpSpeed = 290;
	}
	

	if (pStates[SDL_SCANCODE_S] && !m_IsOnGround && !m_IsOnLadder
		&& m_Velocity.y < 260)
	{
		m_ActionState = ActionState::jumpAttack;
	}

	if (pStates[SDL_SCANCODE_J] && !m_IsAttackPressed)
	{
		m_ActionState = ActionState::attack;
		m_AnimFrame = 0; //otherwise it keeps old animFrame and attack wont work
		m_IsAttackPressed = true;
		m_IsJumpPressed = false;
		m_pDig->Play(false);
	}
	else if (!pStates[SDL_SCANCODE_J]) 
		m_IsAttackPressed = false;

	//ladder stuff
	if (m_IsOnLadder)
	{
		const float ladderSpeed{90};
		if (pStates[SDL_SCANCODE_W])
		{
			if (!m_IsOnTopOfLadder)
			{
				m_ActionState = ActionState::climbing;
				m_Shape.bottom += ladderSpeed * elapsedSec;
			}
			else m_ActionState = ActionState::idle;
		}
		else if (pStates[SDL_SCANCODE_S] && m_ActionState != ActionState::jumpAttack 
			&& (!m_IsOnGround || m_IsOnTopOfLadder))
		{
			m_ActionState = ActionState::climbing;
			m_Shape.bottom -= ladderSpeed * elapsedSec;
		}
		else if ((pStates[SDL_SCANCODE_SPACE] || pStates[SDL_SCANCODE_K])
			&& !m_IsJumpPressed)
		{
			m_ActionState = ActionState::jumping;
			m_IsJumpPressed = true;
		}
	}
}

void Knight::Jump(float elapsedSec)
{
	if (!m_IsOnGround && m_ActionState != ActionState::jumpAttack && m_ActionState != ActionState::attack 
		&& m_ActionState != ActionState::climbing)
	{
		m_ActionState = ActionState::jumping;
	}
}

void Knight::Attack()
{
	if (m_ActionState == ActionState::attack)
	{
		if(m_IsOnGround && (m_Velocity.x == m_HorSpeed || 
				m_Velocity.x == -m_HorSpeed)) m_Velocity.x = 0;

		if (m_AnimFrame >= 2)
		{
			m_HasAttacked = true;
		}
	}
	else m_HasAttacked = false;
}

void Knight::GetHit()
{
	if (!m_IsInvincible && m_ActionState != ActionState::dying)
	{
		m_StunTimer = 0;
		--m_Health;
		m_IsInvincible = true;
		if (m_Health <= 0)
		{
			m_ActionState = ActionState::dying;
			m_Velocity.x = 0;
		}
		else 
		{
			m_ActionState = ActionState::getHit;
		}
		//std::cout << "health is: " << m_Health << "\n";
	}
}

void Knight::KnockBack(float elapsedSec)
{
	Vector2f knockBack{ 180, 210 };

	if (m_IsInvincible)
	{
		m_StunTimer += elapsedSec;

		if (m_ActionState == ActionState::getHit)
		{
			if (!m_HasAppliedKnockBack)
			{
				if (m_IsGoingRight) knockBack.x *= -1;

				m_Velocity.x = knockBack.x;
				m_Velocity.y = knockBack.y;
				m_HasAppliedKnockBack = true;
			}

			if (m_IsOnGround && m_StunTimer >= 0.2f)
			{
				m_ActionState = ActionState::idle;
				m_Velocity.x = 0;
			}
		}

		if (m_StunTimer >= 2.f)
		{
			m_IsInvincible = false;
			m_HasAppliedKnockBack = false;
		}
	}


}

void Knight::Bounce(const float speed)
{
	m_Velocity.y = speed;
	m_pBounce->Play(false);
}

void Knight::Respawn()
{
	m_ActionState = ActionState::idle;
	m_Health = 8;
	m_Shape.left = m_RespawnPos.x;
	m_Shape.bottom = m_RespawnPos.y;
}

void Knight::Healing(int healthBonus)
{
	m_Health += healthBonus;
	if (m_Health > 8) m_Health = 8;
}

void Knight::SetIsOnLadder(const bool isOnLadder, const float midPosX)
{
	m_IsOnLadder = isOnLadder;
	if(m_ActionState == ActionState::climbing)
		m_Shape.left = midPosX - m_Shape.width / 2;
}	

void Knight::SetIsOnTopOfLadder(bool isOnTopOfLadder) // to fix bug at the top of ladder when pressing up
{
	m_IsOnTopOfLadder = isOnTopOfLadder;
}

void Knight::SetIsOnGround(bool isOnGround)
{
	if (!m_IsOnGround && isOnGround)
	{
		m_pLand->Play(false);
	}
	m_IsOnGround = isOnGround;
	
}

void Knight::SetIsLooting(bool isLooting, bool chestFacingLeft)
{
	if (m_ActionState != ActionState::looting
		&& isLooting)
	{
		m_ActionState = ActionState::looting;
		m_Velocity.x = 0;
		m_IsGoingRight = chestFacingLeft;
	}
	else if (m_ActionState == ActionState::looting &&
		!isLooting) m_ActionState = ActionState::finishLooting;
}

void Knight::SetRespawnPos(const Point2f& pos)
{
	m_RespawnPos = pos;
}

void Knight::SetVolume(int volume)
{
	m_pJump->SetVolume(volume);
	m_pLand->SetVolume(volume);
	m_pBounce->SetVolume(volume);
	m_pDig->SetVolume(volume);
}

Rectf& Knight::GetShape()
{
	return m_Shape;
}

Vector2f& Knight::GetVelocity()
{
	return m_Velocity;
}

Rectf Knight::GetHitBox() const
{
	Rectf hitBox{};
	Rectf faker{};
	if (m_ActionState == ActionState::attack && m_AnimFrame == 2)
	{
		hitBox.bottom = m_Shape.bottom + 5;
		hitBox.width = 36;
		hitBox.height = 15;
		if (m_IsGoingRight)
		{
			hitBox.left = m_Shape.left + m_Shape.width / 2;
		}
		else
		{
			hitBox.left = m_Shape.left + m_Shape.width / 2 - hitBox.width;
		}
		return hitBox;
	}
	else if(m_ActionState == ActionState::jumpAttack)
	{
		hitBox.width = m_Shape.width / 2;
		hitBox.height = 20;
		hitBox.left = m_Shape.left + hitBox.width / 2;
		hitBox.bottom = m_Shape.bottom - hitBox.height + 16;
		return hitBox;
	}
	else
	{
		return faker;
	}
}

Rectf Knight::GetHurtBox() const
{
	Rectf hurtBox{ 0, m_Shape.bottom, 0, m_Shape.height };
	const Point2f offset{ 6, 10 };
	hurtBox.left = m_Shape.left + offset.x;
	hurtBox.width = m_Shape.width - offset.x * 2;
	if (m_ActionState == ActionState::jumpAttack)
	{
		hurtBox.bottom = m_Shape.bottom + offset.y;
		hurtBox.height = m_Shape.height - offset.y;
	}
	else
	{
		hurtBox.bottom = m_Shape.bottom;
		hurtBox.height = m_Shape.height;
	}

	return hurtBox;
}

bool Knight::IsKnightRight() const
{
	return m_IsGoingRight;
}

bool Knight::IsJumpAttacking() const
{
	if (m_ActionState == ActionState::jumpAttack)
		return true;
	else return false;
}

bool Knight::IsClimbing() const
{
	if (m_ActionState != ActionState::climbing)
		return false;
	else return true;
}

bool& Knight::HasAttacked()
{
	return m_HasAttacked;
}

bool Knight::IsDead() const
{
	if (m_ActionState == ActionState::dead)
	{
		return true;
	}
	else return false;
}

int Knight::GetHealth() const
{
	return m_Health;
}