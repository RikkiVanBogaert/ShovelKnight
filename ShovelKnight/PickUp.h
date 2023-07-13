#pragma once
#include "Item.h"

class Texture;

class PickUp : public Item
{
public:
	explicit PickUp(const Point2f& pos);
	virtual ~PickUp();
	PickUp(const PickUp& other) = delete;
	PickUp& operator=(const PickUp& other) = delete;
	PickUp(PickUp&& other) = delete;
	PickUp& operator=(PickUp&& other) = delete;

	virtual void Draw() const = 0;
	virtual void Update(float elapsedSec);
	virtual void IsHit() override;
	virtual Rectf GetShape() const override;
	virtual bool GetIsDead() const override;
	virtual bool GetIsActive() const override;
	virtual bool GetIsPickUp() const override;

protected:
	static int m_NrInstances;
	static Texture* m_pPoofTexture;
	enum class State { spawning, active, poof, dead };
	State m_State;

	virtual void ChangeFrame(float elapsedSec, int nrFrames, int fps, bool isLooping);

};

