#pragma once
#include "Interactable.h"

class Texture;

class Platter final : public Interactable
{
public:
	explicit Platter(const Point2f& pos);
	virtual ~Platter();
	Platter(const Platter& other) = delete;
	Platter& operator=(const Platter& other) = delete;
	Platter(Platter&& other) = delete;
	Platter& operator=(Platter&& other) = delete;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec) override;

private:
	static int m_NrInstances;
	static Texture* m_pClosedTexture, *m_pOpenTexture;
	Rectf m_BottomPart;
	float m_DeletePartsTimer;
	Vector2f m_Velocity;
};

