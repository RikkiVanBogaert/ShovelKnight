#pragma once
#include "Enemy.h"

class Bird final : public Enemy
{
public:
	explicit Bird(const Point2f& pos, const Point2f& flyDistance, const Vector2f& speed, bool goingRight = true);
	virtual ~Bird();
	Bird(const Bird& other) = delete;
	Bird& operator=(const Bird& other) = delete;
	Bird(Bird&& other) = delete;
	Bird& operator=(Bird&& other) = delete;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec) override;
	virtual Rectf GetHitBox() const override;


private:
	static int m_NrInstances;
	static Texture* m_pTexture;
	const Point2f m_MinPos;
	Point2f m_MaxPos;

	virtual void ChangeSprites(float elapsedSec) override;
	void SwitchSides();
};
