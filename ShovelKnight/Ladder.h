#pragma once

class Texture;

class Ladder final
{
public:
	explicit Ladder();
	~Ladder();
	Ladder(const Ladder& other) = delete;
	Ladder& operator=(const Ladder& other) = delete;
	Ladder(Ladder&& other) = delete;
	Ladder& operator=(Ladder&& other) = delete;

	void Draw() const;
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, const bool isClimbing);
	bool IsOnGround(const Rectf& actorShape, const bool isClimbing) const;
	bool IsOnLadder(const Rectf& actorShape, const Vector2f& actorVelocity);
	float GetMidPosCurrentLadder() const;
	bool GetIsOnTopLadder() const;

private:
	Texture* m_pLadderTexture;
	std::vector<std::vector<Point2f>> m_Vertices;
	int m_CurrentLadderIndex;
	bool m_IsOnTop;
};

