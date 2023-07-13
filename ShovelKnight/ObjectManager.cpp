#include "pch.h"
#include "ObjectManager.h"
#include "Enemy.h"
#include "Beeto.h"
#include "Skeleton.h"
#include "Blorb.h"
#include "Bird.h"
#include "Ladder.h"
#include "Item.h"
#include "Bubble.h"
#include "Rock.h"
#include "BreakableWall.h"
#include "Gem.h"
#include "DirtPile.h"
#include "Platform.h"
#include "Chest.h"
#include "Checkpoint.h"
#include "MusicSheet.h"
#include "Platter.h"
#include "Dragon.h"

ObjectManager::ObjectManager()
{
	for (int i{}; i < 24; ++i)
	{
		m_VisitedStages.push_back(false);
	}
}


void ObjectManager::CreateObjects(std::vector <Item*>& items, std::vector <Enemy*>& enemies, int currentStage)
{
	if (currentStage >= 0)
	{
		if (!m_VisitedStages[currentStage])
		{
			switch (currentStage)
			{
			case 0:
				//items
				items.push_back(new DirtPile{ Point2f(370, 640) });
				items.push_back(new Rock{ Point2f(936, 703), true, true });
				items.push_back(new DirtPile{ Point2f(955, 640), 0 });
				items.push_back(new Rock{ Point2f(1200, 704), true, true });
				CreateGridRocks(items, Point2f(1568, 672), 1, 3, true);

				enemies.push_back(new Beeto{ Point2f(560, 640) });
				enemies.push_back(new Beeto{ Point2f(720, 710) });
				enemies.push_back(new Beeto{ Point2f(810, 640) });
				enemies.push_back(new Beeto{ Point2f(950, 640) });
				enemies.push_back(new Beeto{ Point2f(1110, 640) });
				enemies.push_back(new Beeto{ Point2f(1344, 677) });

				break;
			case 1:

				//items
				items.push_back(new Rock(Point2f(1728, 672), true));
				CreateGridRocks(items, Point2f(1824, 672), 1, 2, true);
				items.push_back(new DirtPile{ Point2f(1907, 640) });

				enemies.push_back(new Beeto{ Point2f(1870, 640) });

				break;
			case 2:

				//items
				items.push_back(new Bubble{ Point2f(2205, 570), 670 });

				break;
			case 3:

				items.push_back(new Platform{ Point2f(2140, 929), Point2f(2310, 929), false });
				items.push_back(new Checkpoint{ Point2f(2180, 847) });
				//beetle

				break;
			case 4:
				items.push_back(new Rock{ Point2f(2480, 848), true });
				items.push_back(new Rock{ Point2f(2480, 880), false });
				items.push_back(new Rock{ Point2f(2752, 848), false });
				items.push_back(new Rock{ Point2f(2767, 848), true });
				items.push_back(new Platform{ Point2f(2552, 833), Point2f(2712, 833), false });
				//gem

				break;
			case 5:

				enemies.push_back(new Dragon{ Point2f(3000, 815) });
				break;

			case 6:

				CreateGridRocks(items, Point2f(2864, 624), 2, 3, false);
				//skulls

				enemies.push_back(new Skeleton{ Point2f(2950, 673) });

				break;
			case 7:

				items.push_back(new DirtPile{ Point2f(2886, 415) });
				items.push_back(new BreakableWall{ Point2f(2960, 415), 0 });
				items.push_back(new BreakableWall{ Point2f(3040, 544), 1 });
				CreateGridRocks(items, Point2f(3168, 415), 1, 3, true);

				break;
			case 8:

				items.push_back(new Rock{ Point2f(2961, 224), false });
				items.push_back(new BreakableWall{ Point2f(2800, 224), 2 });

				enemies.push_back(new Skeleton{ Point2f(2870, 224) });
				enemies.push_back(new Beeto{ Point2f(3090, 224) });

				break;
			case 9:

				items.push_back(new DirtPile{ Point2f(2615, 224) });
				items.push_back(new MusicSheet{ Point2f(2470, 255) });
				//skulls, gems, 

				break;
			case 10:

				items.push_back(new DirtPile{ Point2f(3441, 340), 90 });
				CreateGridRocks(items, Point2f(3392, 288), 3, 1, true);
				CreateGridRocks(items, Point2f(3488, 224), 2, 2, true);
				//rock pile
				CreateTriRocks(items, Point2f(3648, 224));
				items.push_back(new Rock{ Point2f(3680, 224), true, true });
				CreateTriRocks(items, Point2f(3680, 256));
				items.push_back(new Rock{ Point2f(3712, 256), true, true });
				CreateTriRocks(items, Point2f(3712, 288));
				CreateGridRocks(items, Point2f(3744, 256), 1, 4, false); //vertical line
				items.push_back(new Rock{ Point2f(3760, 271), true, true }); //center rock
				CreateGridRocks(items, Point2f(3792, 256), 1, 4, false); //vert line
				CreateGridRocks(items, Point2f(3760, 256), 2, 1, false); //hor line 
				CreateGridRocks(items, Point2f(3760, 304), 2, 1, false); //hor line
				CreateGridRocks(items, Point2f(3760, 224), 1, 2, false);
				items.push_back(new Rock{ Point2f(3776, 224), true , true });
				CreateGridRocks(items, Point2f(3808, 224), 2, 3, false);
				items.push_back(new Rock{ Point2f(3840, 224), true , true });
				CreateGridRocks(items, Point2f(3840, 256), 2, 2, false);
				items.push_back(new Rock{ Point2f(3840, 224), true , true });
				CreateGridRocks(items, Point2f(3872, 224), 1, 3, false);
				CreateGridRocks(items, Point2f(3888, 224), 1, 2, false);
				items.push_back(new Rock{ Point2f(3904, 224), false });
				items.push_back(new Rock{ Point2f(3808, 288), true, true });
				items.push_back(new Rock{ Point2f(3840, 288), true, true });
				//end pile
				items.push_back(new BreakableWall{ Point2f(3841, 336), 3 });
				items.push_back(new Platter{ Point2f(3721, 224) });
				//dinner plate

				enemies.push_back(new Blorb{ Point2f(3400, 224) });
				enemies.push_back(new Bird{ Point2f(3580, 280), Point2f(70, 30), Vector2f(22, 10) });
				enemies.push_back(new Blorb{ Point2f(3780, 271) });

				break;
			case 11:

				items.push_back(new Bubble{ Point2f(4233, 150), 220 });
				CreateGridRocks(items, Point2f(4288, 224), 4, 2, false);
				items.push_back(new Checkpoint{ Point2f(4100, 223) });

				enemies.push_back(new Blorb{ Point2f(4400, 256) });

				break;
			case 12:

				items.push_back(new Platform{ Point2f(4202, 480), Point2f(4375, 480), false });
				items.push_back(new Platform{ Point2f(4110, 440), Point2f(4110, 500), true });
				items.push_back(new BreakableWall{ Point2f(4432, 496), 2 });

				enemies.push_back(new Bird{ Point2f(4170, 500), Point2f(20, 20), Vector2f(5, 5) });

				break;
			case 13:

				items.push_back(new Bubble{ Point2f(4560, 375), 430 });
				items.push_back(new Bubble{ Point2f(4620, 375), 440 });
				items.push_back(new Rock{ Point2f(4720, 464), false , true });
				items.push_back(new Rock{ Point2f(4736, 464), true , true });
				items.push_back(new Bubble{ Point2f(4810, 375), 440 });
				items.push_back(new Bubble{ Point2f(4880, 375), 430 });
				items.push_back(new MusicSheet{ Point2f(4975, 525) });

				break;
			case 14:
				 
				items.push_back(new DirtPile{ Point2f(4270, 720) });
				items.push_back(new BreakableWall{ Point2f(4192, 656), 4 });
				items.push_back(new BreakableWall{ Point2f(4224, 720), 5 });

				enemies.push_back(new Bird{ Point2f(4120, 680), Point2f(0, 60), Vector2f(0, 20) });

				break;
			case 15:

				items.push_back(new Chest{ Point2f(4375, 832) });
				items.push_back(new Checkpoint{ Point2f(4330, 911) });

				break;
			case 16:

				items.push_back(new Platform{ Point2f(5055, 830), Point2f(0, 965), true });

				enemies.push_back(new Bird{ Point2f(4720, 940), Point2f(300, 0), Vector2f(-70, 0), false });
				enemies.push_back(new Bird{ Point2f(4950, 900), Point2f(400, 0), Vector2f(-70, 0), false });
				enemies.push_back(new Bird{ Point2f(5240, 860), Point2f(500, 0), Vector2f(-70, 0), false });
				enemies.push_back(new Blorb{ Point2f(4740, 880) });

				break;
			case 17:

				CreateGridRocks(items, Point2f(5296, 912), 2, 1, true);

				enemies.push_back(new Dragon{ Point2f(5450, 820) });

				break;
			case 18:

				enemies.push_back(new Skeleton{ Point2f(5850, 860) });

				break;
			case 19:

				CreateGridRocks(items, Point2f(5872, 1088), 6, 5, false);
				items.push_back(new Rock{ Point2f(5840, 1136), true });
				items.push_back(new DirtPile{ Point2f(5670, 1104), 0 });
				items.push_back(new BreakableWall{ Point2f(5662, 1040), 6 });

				enemies.push_back(new Blorb{ Point2f(5900, 1040) });
				enemies.push_back(new Blorb{ Point2f(5810, 1040) });

				break;
			case 20:

				enemies.push_back(new Bird{ Point2f(5710, 1260), Point2f(0, 100), Vector2f(0, 25) });
				enemies.push_back(new Beeto{ Point2f(5880, 1310) });
				enemies.push_back(new Beeto{ Point2f(5920, 1280) });

				break;
			case 21:

				items.push_back(new Bubble{ Point2f(5370, 1200), 1265 });
				items.push_back(new Chest{ Point2f(5200, 1247), false });

				break;
			case 22:
				items.push_back(new BreakableWall{ Point2f(6160, 1264), 7 });
				items.push_back(new Checkpoint{ Point2f(6230, 1326) });
				break;
			}
			m_VisitedStages[currentStage] = true;
		}
	}
}

void ObjectManager::DeleteObjects(std::vector <Item*>& items, std::vector <Enemy*>& enemies, const Rectf& actorShape)
{
	const float deleteDistance{1600};

	for (int i{ }; i < items.size(); ++i)
	{
		if (items[i])
		{
			if (abs(actorShape.left - items[i]->GetShape().left) > deleteDistance &&
				typeid(*items[i]) != typeid(Checkpoint) &&
				typeid(*items[i]) != typeid(Platform))
			{
				delete items[i];
				items[i] = nullptr;
				items[i] = items[items.size() - 1];
				items.pop_back();
			}
		}
	}

	for (int i{ }; i < enemies.size(); ++i)
	{
		if (enemies[i])
		{
			if (abs(actorShape.left - enemies[i]->GetShape().left) > deleteDistance)
			{
				delete enemies[i];
				enemies[i] = nullptr;
				enemies[i] = enemies[enemies.size() - 1];
				enemies.pop_back();
			}
		}
	}
}

void ObjectManager::CreateGridRocks(std::vector <Item*>& items, const Point2f& bottomLeft, int cols, int rows, bool isBig)
{
	Point2f pos{bottomLeft};
	for (int i{1}; i <= cols * rows; ++i)
	{
		items.push_back(new Rock{ pos, isBig });
		const float width{ items[items.size() - 1]->GetShape().width };
		const float height{ items[items.size() - 1]->GetShape().height };
		if (i % cols == 0)
		{
			pos.y += height;
			pos.x = bottomLeft.x;
		}
		else
		{
			pos.x += width;
		}
	}
}

void ObjectManager::CreateTriRocks(std::vector <Item*>& items, const Point2f& bottomLeft)
{
	items.push_back(new Rock{ bottomLeft, false });
	const float size{ items[items.size() - 1]->GetShape().width };
	items.push_back(new Rock{ Point2f(bottomLeft.x + size, bottomLeft.y ), false });
	items.push_back(new Rock{ Point2f(bottomLeft.x + size, bottomLeft.y + size), false });
}