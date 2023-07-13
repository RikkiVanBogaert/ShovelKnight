#pragma once
#include "Enemy.h"

class Skeleton final : public Enemy
{
public:
	explicit Skeleton(const Point2f& pos);
	virtual ~Skeleton();
	Skeleton(const Skeleton& other) = delete;
	Skeleton& operator=(const Skeleton& other) = delete;
	Skeleton(Skeleton&& other) = delete;
	Skeleton& operator=(Skeleton&& other) = delete;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec) override;
	void UpdateState(float elapsedSec, const Rectf& actorShape);
	virtual Rectf GetHitBox() const;

private:
	static int m_NrInstances;
	static Texture* m_pTexture;
	float m_AttackTimer;

	virtual void ChangeSprites(float elapsedSec) override;
	virtual void InHit(float elapsedSec, float stunTime) override;
};

