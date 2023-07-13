#pragma once
#include "Enemy.h"

class Blorb final : public Enemy
{
public:
	explicit Blorb(const Point2f& pos);
	virtual ~Blorb();
	Blorb(const Blorb& other) = delete;
	Blorb& operator=(const Blorb& other) = delete;
	Blorb(Blorb&& other) = delete;
	Blorb& operator=(Blorb&& other) = delete;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec) override;
	virtual Rectf GetHitBox() const override;


private:
	static int m_NrInstances;
	static Texture* m_pTexture;

	virtual void ChangeSprites(float elapsedSec) override;
};

