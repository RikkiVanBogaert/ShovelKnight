#pragma once
#include "PickUp.h"

class Texture;

class Chicken final : public PickUp
{
public:
	explicit Chicken(const Point2f& pos);
	virtual ~Chicken();
	Chicken(const Chicken& other) = delete;
	Chicken& operator=(const Chicken& other) = delete;
	Chicken(Chicken&& other) = delete;
	Chicken& operator=(Chicken&& other) = delete;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec) override;
	void SetIsOnGround(bool isOnGround);
	Rectf& GetLevelCollisionShape();
	Vector2f& GetVelocity();

private:
	static int m_NrInstances;
	static Texture* m_pTexture;
	Vector2f m_Velocity;
	bool m_IsOnGround;

	void LilJump(float elapsedSec);
};

