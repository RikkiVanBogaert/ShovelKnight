#include "pch.h"
#include "Item.h"

Item::Item(const Point2f& pos):
	m_Shape{pos.x, pos.y, 10, 10},
	m_SrcRect{},
	m_AnimTime{},
	m_AnimFrame{}
{}

Item::~Item()
{}

void Item::SpawnGems(std::vector<Item*>& items, bool isKnightRight)
{}

Rectf Item::GetShape() const
{
	return m_Shape;
}

bool Item::GetIsActive() const
{
	return true;
}

bool Item::GetIsPickUp() const
{
	return false;
}