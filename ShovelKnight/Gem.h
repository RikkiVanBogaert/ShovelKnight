#pragma once
#include "PickUp.h"

class SoundEffect;
class Texture;

class Gem final : public PickUp
{
public:
	explicit Gem(const Point2f& pos, const Vector2f& velocity = Vector2f(0,0), bool isBigDiamond = false);
	virtual ~Gem();
	Gem(const Gem& other) = delete;
	Gem& operator=(const Gem& other) = delete;
	Gem(Gem&& other) = delete;
	Gem& operator=(Gem&& other) = delete;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec) override;
	void LilJump( float elapsedSec);
	virtual void IsHit() override;
	void SetIsOnGround(bool isOnGround);
	void SetVolume(int volume);
	Rectf& GetLevelCollisionShape(); // for level collision
	Vector2f& GetVelocity();
	int GetValue() const;

private:
	static int m_NrInstances;
	static Texture* m_pGemTexture;
	static SoundEffect* m_pCollected;
	int m_Value;
	Vector2f m_Velocity;
	bool m_IsOnGround;

	void DrawHurtBox() const;
	void GetRandomGem();
};

