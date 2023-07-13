#pragma once
#include <vector>
#include "structs.h"
#include "Texture.h"
#include "Vector2f.h"

class Waterfall;

class Level final
{
public:
	Level();
	~Level();
	Level(const Level& other) = delete;
	Level& operator=(const Level& other) = delete;
	Level(Level&& other) = delete;
	Level& operator=(Level&& other) = delete;

	void Draw(const Vector2f& transform, float scale, const Point2f& playerPos) const;
	void Update(float elapsedSec);
	void DrawClouds(const Rectf& window) const;
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, bool isNPC);
	bool IsOnGround(const Rectf& actorShape) const;
	Rectf GetBoundaries() const;
	bool IsInSpikes(const Rectf& actorShape);

private:
	std::vector<std::vector<Point2f>> m_Vertices, m_SpikeVertices;
	const Texture m_FloorTexture, m_CloudsTexture, 
		m_BG1, m_BG2, m_BG3, m_CaveTexture, m_CastleTexture;
	Rectf m_Boundaries;
	Waterfall* m_pWaterfall;

	void CheckOutOfBounds(Rectf& actorShape, Vector2f& actorVelocity, const bool isNPC);
};

