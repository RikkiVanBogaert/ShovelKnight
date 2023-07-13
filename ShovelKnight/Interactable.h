#pragma once
#include "Item.h"

class Interactable : public Item
{
public:
	explicit Interactable(const Point2f& pos);
	virtual ~Interactable();
	Interactable(const Interactable& other) = delete;
	Interactable& operator=(const Interactable& other) = delete;
	Interactable(Interactable&& other) = delete;
	Interactable& operator=(Interactable&& other) = delete;

	virtual void Draw() const = 0;
	virtual void Update(float elapsedSec) = 0;
	virtual void IsHit();
	virtual void SpawnGems(std::vector<Item*>& items, bool isKnightRight);
	virtual Rectf GetShape() const override;
	virtual bool GetIsDead() const override;
	virtual bool GetIsActive() const override;
	virtual bool GetIsPickUp() const override;

protected:
	enum class State { active, inactive, dead };
	State m_State;

	void ChangeFrame(float elapsedSec, int nrFrames, int fps, bool isLooping);
};

