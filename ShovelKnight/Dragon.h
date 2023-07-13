#pragma once
#include "Enemy.h"
#include "DragonBubbles.h"

class Dragon final : public Enemy
{
public:
	explicit Dragon(const Point2f& pos);
	virtual ~Dragon();
	Dragon(const Dragon& other) = delete;
	Dragon& operator=(const Dragon& other) = delete;
	Dragon(Dragon&& other) = delete;
	Dragon& operator=(Dragon&& other) = delete;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec) override;
	bool Collision(const Rectf& actorShape) const;
	virtual bool CheckHit(const Rectf& actorHitBox, bool hasKnightAttacked);
	virtual void GetHit() override;
	bool CheckHitBubbles(const Rectf& actorHitBox);
	void SpawnGemsDragon(std::vector<Item*>& items);
	bool IsActive() const;
	virtual bool GetIsDead() const override;

private:
	static int m_NrInstances;
	static Texture* m_pTexture;
	enum class DragonState { idle, forward, backward, attack, poof, dead };
	DragonState m_DragonState, m_TempState;
	Rectf m_Head, m_Body;
	bool m_StateCycleDone;
	int m_Position;

	std::vector<DragonBubbles> m_Bubbles;
	int m_CurrBubbleIndex;
	int m_TempAnimFrame;


    void ChangeSprites(float elapsedSec);
	void UpdateDragonSprites(float elapsedSec, const bool isLooping, const int amountFrames, int fps = 5);
	void UpdateState(float elapsedSec);
	bool CheckBodyCollision(const Rectf& actorShape) const;
	void DrawHitBoxes() const;

	//bubblestuff
	void ActivateBubbles();
	void IncrementBubbleIndex();
	void DrawBubbles() const;
	void UpdateBubbles(float elapsedSec);
	bool BubbleCollision(const Rectf& actorShape) const;
	
};

