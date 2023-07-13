#pragma once
#include "Interactable.h"

class Texture;

class Rock final : public Interactable
{
public:
	explicit Rock(const Point2f& pos, bool isBig, bool spawnGem = false);
	virtual ~Rock();
	Rock(const Rock& other) = delete;
	Rock& operator=(const Rock& other) = delete;
	Rock(Rock&& other) = delete;
	Rock& operator=(Rock&& other) = delete;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec) override;
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, bool isNPC);
	virtual void SpawnGems(std::vector<Item*>& items, bool isKnightRight) override;
	bool IsOnGround(const Rectf& actorShape) const;

private:
	static int m_NrInstancesBig, m_NrInstancesSmall;
	static Texture* m_pBigRockTexture, *m_pSmallRockTexture, 
		*m_pBigPopTexture, *m_pSmallPopTexture;
	const bool m_SpawnsGem;
	const bool m_IsBig;

};

