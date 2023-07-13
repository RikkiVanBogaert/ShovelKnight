#pragma once
#include "Texture.h"

class DragonBubbles final
{
public:
	explicit DragonBubbles(const Point2f& pos);

	void Draw() const;
	void Update(float elapsedSec);
	void IsHit();
	void Activate(const Point2f& pos);
	Rectf GetShape() const;
	bool GetIsDead() const;
	bool GetIsActive() const;

private:
	Texture m_BubbleTexture, m_BubblePopTexture;
	enum class State { active, popped, dead };
	State m_State;
	Point2f m_BeginPos;
	Rectf m_Shape;
	Rectf m_SrcRect;
	const Point2f m_Speed;
	float m_AnimTime;
	int m_AnimFrame;
	enum class ActionState {inactive, horizontal, vertical};
	ActionState m_ActionState;
	const int m_MaxX;

    void ChangeFrame(float elapsedSec, int nrFrames, int fps, bool isLooping);
	void Movement(float elapsedSec);
	void DrawHitBox() const;
};

