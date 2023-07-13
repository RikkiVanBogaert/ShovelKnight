#pragma once

class Item
{
public:
	explicit Item(const Point2f& pos);
	virtual ~Item();
	Item(const Item& other) = delete;
	Item& operator=(const Item& other) = delete;
	Item(Item&& other) = delete;
	Item& operator=(Item&& other) = delete;

	virtual void Draw() const = 0;
	virtual void Update(float elapsedSec) = 0;
	virtual void IsHit() = 0;
	virtual void SpawnGems(std::vector<Item*>& items, bool isKnightRight);
	virtual Rectf GetShape() const;
	virtual bool GetIsDead() const = 0;
	virtual bool GetIsActive() const;
	virtual bool GetIsPickUp() const;

protected:
	Rectf m_Shape;
	Rectf m_SrcRect;
	float m_AnimTime;
	int m_AnimFrame;
};

