#pragma once
class Texture;
class Item;

class Enemy
{
public:
	explicit Enemy(const Point2f& pos, int health, const Vector2f& velocity = Vector2f(0,0));
	virtual ~Enemy();
	Enemy(const Enemy& other) = delete;
	Enemy& operator=(const Enemy& other) = delete;
	Enemy(Enemy&& other) = delete;
	Enemy& operator=(Enemy&& other) = delete;

	virtual void Draw() const = 0;
	virtual void Update(float elapsedSec);
	virtual void GetHit();
	virtual void SpawnGems(std::vector<Item*>& items);

	virtual Rectf GetHitBox() const;
	virtual Rectf& GetShape();
	Vector2f& GetVelocity();
	bool GetIsDying() const;
	void SetKnightIsRight(const bool isKnightRight);
	void SetIsOnGround(bool isOnGround);
	virtual bool GetIsDead() const;

protected:
	static int m_NrInstancesPoof;
	static Texture* m_pPoofTxt;

	Rectf m_Shape;
	Rectf m_SrcRect;
	enum class ActionState { moving, idle, attack, getHit, poof, dead };
	ActionState m_ActionState, m_TempState;
	float m_AnimTime;
	int m_AnimFrame;
	const float m_Gravity;
	Vector2f m_Velocity;
	bool m_IsGoingRight;
	bool m_KnightIsRight;
	bool m_IsOnGround;
	float m_StunTimer;
	bool m_IsDead;
	int m_Health;
	bool m_HasSpawnedGems;

	virtual void InHit(float elapsedSec, float stunTime = 0.5f);
	virtual void UpdateSprites(float elapsedSec, int amountFrames, int fps = 5);
	virtual void ChangeSprites(float elapsedSec) = 0;
	void DieWhenPoofDone();

};

