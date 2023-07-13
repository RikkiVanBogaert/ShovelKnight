#pragma once
#include "Interactable.h"

class Texture;
class Enemy;
class SoundStream;

class BreakableWall final : public Interactable
{
public:
	explicit BreakableWall(const Point2f& pos, int typeWall);
	virtual ~BreakableWall();
	BreakableWall(const BreakableWall& other) = delete;
	BreakableWall& operator=(const BreakableWall& other) = delete;
	BreakableWall(BreakableWall&& other) = delete;
	BreakableWall& operator=(BreakableWall&& other) = delete;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec) override;
	virtual void SpawnGems(std::vector<Item*>& items, bool isKnightRight) override;
	void SpawnEnemies(std::vector<Enemy*>& enemies);
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, bool isNPC);
	virtual void IsHit() override;
	bool IsOnGround(const Rectf& actorShape) const;

private:
	static int m_NrInstances;
	static Texture* m_pWallTexture, *m_pExplosionTexture;
	static SoundStream* m_pWallBreak;
	const int m_TypeWall;

	void GiveTypeWall(int typeWall);

};

