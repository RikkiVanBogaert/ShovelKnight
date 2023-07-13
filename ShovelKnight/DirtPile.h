#pragma once
#include "Interactable.h"

class Texture;

class DirtPile final : public Interactable
{
public:
	explicit DirtPile(const Point2f& pos, float angle = 0);
	virtual ~DirtPile();
	DirtPile(const DirtPile& other) = delete;
	DirtPile& operator=(const DirtPile& other) = delete;
	DirtPile(DirtPile&& other) = delete;
	DirtPile& operator=(DirtPile&& other) = delete;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec) override;
	virtual void IsHit() override;
	virtual void SpawnGems(std::vector<Item*>& items, bool isKnightRight) override;
	virtual Rectf GetShape() const override;
	virtual bool GetIsDead() const override;
	virtual bool GetIsActive() const override;

private:
	static int m_NrInstances;
	static Texture* m_pDirtPileTexture;
	int m_SizePile;
	const float m_Angle;
};

