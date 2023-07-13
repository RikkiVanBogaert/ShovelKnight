#pragma once
#include "Interactable.h"

class Texture;

class Bubble final : public Interactable
{
public:
	explicit Bubble(const Point2f& pos, float maxY);
	virtual ~Bubble();
	Bubble(const Bubble& other) = delete;
	Bubble& operator=(const Bubble& other) = delete;
	Bubble(Bubble&& other) = delete;
	Bubble& operator=(Bubble&& other) = delete;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec) override;


private:
	static int m_NrInstances;
	static Texture* m_pBubbleTexture, *m_pBubblePopTexture;
	const float m_UpSpeed;
	const float m_BeginY;
	const float m_MaxY;

	void Respawn();

};

