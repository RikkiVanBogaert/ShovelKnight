#pragma once
#include "PickUp.h"

class Texture;

class MusicSheet final : public PickUp
{
public:
	explicit MusicSheet(const Point2f& pos);
	virtual ~MusicSheet();
	MusicSheet(const MusicSheet& other) = delete;
	MusicSheet& operator=(const MusicSheet& other) = delete;
	MusicSheet(MusicSheet&& other) = delete;
	MusicSheet& operator=(MusicSheet&& other) = delete;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec) override;

private:
	static int m_NrInstances;
	static Texture* m_pTexture;
	const float m_MinY, m_MaxY;
	bool m_IsGoingUp;

	void MoveUpDown(float elaspedSec);
};

