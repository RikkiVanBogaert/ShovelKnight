#pragma once
#include "Item.h"

class Texture;

class Chest final : public Item
{
public:
	explicit Chest(const Point2f& pos, bool isFacingLeft = true);
	virtual ~Chest();
	Chest(const Chest& other) = delete;
	Chest& operator=(const Chest& other) = delete;
	Chest(Chest&& other) = delete;
	Chest& operator=(Chest&& other) = delete;

	virtual void Draw() const;
	virtual void Update(float elapsedSec) override;
	virtual void IsHit() override;
	virtual void SpawnGems(std::vector<Item*>& items, bool isKnightRight) override;
	virtual bool GetIsDead() const override;
	virtual bool GetIsActive() const override;
	bool GetIsOpen() const;
	bool GetIsFacingLeft() const;

private:
	static int m_NrInstances;
	static Texture* m_pChestTexture;
	enum class State{closed, open, empty};
	State m_State;
	const float m_LootTime;
	float m_LootTimer;
	const bool m_IsFacingLeft;

	void UpdateSprite();
};
