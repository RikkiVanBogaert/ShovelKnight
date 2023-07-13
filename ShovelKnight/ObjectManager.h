#pragma once
#include <vector>

class Item;
class Enemy;

class ObjectManager final
{
public:
	explicit ObjectManager();

	void CreateObjects(std::vector <Item*>& items, std::vector <Enemy*>& enemies, int currentStage);
	void DeleteObjects(std::vector <Item*>& items, std::vector <Enemy*>& enemies, const Rectf& actorShape);


private:
	std::vector<bool> m_VisitedStages;

	void CreateGridRocks(std::vector <Item*>& items, const Point2f& bottomLeft, int cols, int rows, bool isBig);
	void CreateTriRocks(std::vector <Item*>& items, const Point2f& bottomLeft);
};

