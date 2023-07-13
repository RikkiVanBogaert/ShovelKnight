#pragma once

class Texture;

class Waterfall
{
public:
	explicit Waterfall();
	~Waterfall();
	Waterfall(const Waterfall& other) = delete;
	Waterfall& operator=(const Waterfall& other) = delete;
	Waterfall(Waterfall&& other) = delete;
	Waterfall& operator=(Waterfall&& other) = delete;

	void Draw(const Point2f& playerPos);
	void Update(float elapsedSec);

private:
	static int m_NrInstances;
	static Texture* m_pTexture;
	std::vector < std::vector<Point2f>> m_Vertices;
	Rectf m_SrcRect;
	int m_AnimFrame;
	float m_AnimTime;

	void DrawGrid(const Point2f& pos, int cols, int rows) const;
	void ChangeColsRows(int i, int& col, int& row);
};

