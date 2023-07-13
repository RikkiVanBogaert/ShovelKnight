#pragma once
#include "Enemy.h"

class Beeto final : public Enemy
{
public:
	explicit Beeto(const Point2f& pos);
	virtual ~Beeto();
	Beeto(const Beeto& other) = delete;
	Beeto& operator=(const Beeto& other) = delete;
	Beeto(Beeto&& other) = delete;
	Beeto& operator=(Beeto&& other) = delete;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec) override;
	virtual Rectf GetHitBox() const override;

private:
	static int m_NrInstances;
	static Texture* m_pTexture;

	virtual void InHit(float elapsedSec, float stunTime = 0.5f) override;
	virtual void ChangeSprites(float elapsedSec) override;
};

