#pragma once
#include "PickUp.h"

class Texture;

class Checkpoint final : public PickUp
{
public:
	Checkpoint(const Point2f& pos);
	virtual ~Checkpoint();
	Checkpoint(const Checkpoint& other) = delete;
	Checkpoint& operator=(const Checkpoint& other) = delete;
	Checkpoint(Checkpoint&& other) = delete;
	Checkpoint& operator=(Checkpoint&& other) = delete;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec) override;
	virtual void IsHit() override;
	virtual bool GetIsDead() const override;
	virtual bool GetIsActive() const override;

private:
	static int m_NrInstances;
	static Texture* m_pTexture;
	enum class State{inactive, activated, active};
	State m_State;
	Rectf m_SrcRectBall;

	virtual void ChangeFrame(float elapsedSec, int nrFrames, int fps, bool isLooping) override;
	void UpdateSprites(float elapsedSec);
};

