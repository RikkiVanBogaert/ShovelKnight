#pragma once
#include "Item.h"

class Texture;

class Platform final : public Item
{
public:
	explicit Platform(const Point2f& minPos, const Point2f& maxPos, bool isVertical);
	virtual ~Platform();
	Platform(const Platform& other) = delete;
	Platform& operator=(const Platform& other) = delete;
	Platform(Platform&& other) = delete;
	Platform& operator=(Platform&& other) = delete;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec) override;
	virtual void IsHit() override;
	virtual bool GetIsDead() const override;
	virtual bool GetIsActive() const override;
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, float elapsedSec);
	bool IsOnGround(const Rectf& actorShape) const;

private:
	static int m_NrInstances;
	static Texture* m_pTexture;
	const Point2f m_MinPos, m_MaxPos;
	float m_Speed;
	const bool m_IsVertical;

};

