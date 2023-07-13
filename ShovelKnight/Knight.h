#pragma once
#include "SoundEffect.h"

struct Vector2f;
class Texture;

class Knight final
{
public:
	Knight(const Point2f& pos);
	~Knight();
	Knight(const Knight& other) = delete;
	Knight& operator=(const Knight& other) = delete;
	Knight(Knight&& other) = delete;
	Knight& operator=(Knight&& other) = delete;

	void Draw() const;
	void Update(float elapsedSec);
	void GetHit();	
	void Bounce(const float speed = 240);
	void Respawn();
	void Healing(int health);

	void SetIsOnLadder( bool isOnLadder, float midPosX);
	void SetIsOnTopOfLadder(bool isOnTopOfLadder);
	void SetIsOnGround( bool isOnGround);
	void SetIsLooting(bool isLooting, bool chestFacingLeft);
	void SetRespawnPos(const Point2f& pos);
	void SetVolume(int volume);

	bool IsJumpAttacking() const;
	Rectf& GetShape();
	Vector2f& GetVelocity();
	Rectf GetHitBox() const;
	Rectf GetHurtBox() const;
	bool IsKnightRight() const;
	bool IsClimbing() const;
	bool& HasAttacked();
	bool IsDead() const;
	int GetHealth() const;

private:
	Texture* m_pSpriteSheet;
	Rectf m_Shape;
	enum class ActionState{idle, moving, jumping, jumpAttack, attack, getHit, 
	 climbing, looting, finishLooting, dying, dead};
	ActionState m_ActionState;
	int m_Health;
	Vector2f m_Velocity;
	const float m_HorSpeed;
	const float m_Gravity;
	float m_JumpSpeed;
	Point2f m_RespawnPos;
	bool m_IsGoingRight;
	bool m_IsOnGround;
	float m_StunTimer;
	bool m_IsInvincible;
	bool m_IsOnLadder;
	bool m_IsOnTopOfLadder;
	bool m_HasAttacked;
	bool m_HasAppliedKnockBack;
	bool m_IsJumpPressed;
	bool m_IsAttackPressed;
	//sprite stuff
	Rectf m_SrcRect;
	const int m_NrFramesPerSec;
	float m_AnimTime;
	int m_AnimFrame;
	ActionState m_TempActionState;
	//sound
	SoundEffect* m_pJump, *m_pLand, *m_pDig, *m_pBounce;

	void HandleKeyInputs(float elapsedSec);
	void Jump(float elapsedSec);
	void Attack();
	void ChangeSpriteCoordinates(float elapsedSec) ; 
	void ChangeFrame(float elapsedSec, int nrFrames, int fps, bool isLooping);
	void ResetAnimFrame();
	void KnockBack(float elapsedSec);
};

