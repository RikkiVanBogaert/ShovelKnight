#include "pch.h"
#include "Waterfall.h"
#include "Texture.h"
#include "SVGParser.h"

int Waterfall::m_NrInstances{};
Texture* Waterfall::m_pTexture{};

Waterfall::Waterfall():
	m_SrcRect{},
	m_AnimFrame{},
	m_AnimTime{}
{
	++m_NrInstances;
	if (!m_pTexture) m_pTexture = new Texture{ "Resources/Images/Waterfall.png" };

	SVGParser::GetVerticesFromSvgFile("Resources/Images/Level/Waterfall.svg", m_Vertices);
	m_SrcRect.width = m_pTexture->GetWidth() / 3;
	m_SrcRect.height = m_pTexture->GetHeight();
}

Waterfall::~Waterfall()
{
	--m_NrInstances;
	if (m_NrInstances <= 0)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	for (size_t i{}; i < m_Vertices.size(); ++i)
	{
		m_Vertices.pop_back();
	}
}


void Waterfall::Draw(const Point2f& playerPos)
{
	int col{};
	int row{};
	for (int i{int(m_Vertices.size() - 1)}; i < m_Vertices.size(); ++i)
	{
		if (utils::GetDistance(m_Vertices[i][0], playerPos) < 1000)
		{
			ChangeColsRows(i, col, row);
			DrawGrid(m_Vertices[i][0], 3, 21);
		}
	}
}

void Waterfall::Update(float elapsedSec)
{
	m_AnimTime += elapsedSec;
	
	if (m_AnimTime >= 1.0f / 5)
	{
		++m_AnimFrame;
		m_AnimTime = 0;
	}
	m_AnimFrame %= 3;
	
	m_SrcRect.left = m_AnimFrame * m_SrcRect.width;
}

void Waterfall::DrawGrid( const Point2f& beginPos, int cols, int rows) const
{
	Point2f pos{ beginPos };

	for (int i{ 1 }; i <= cols * rows; ++i)
	{
		m_pTexture->Draw(pos, m_SrcRect);
		if (i % cols == 0)
		{
			pos.y += m_pTexture->GetHeight();
			pos.x = beginPos.x;
		}
		else
		{
			pos.x += m_pTexture->GetWidth() / 3;
		}
	}
}

void Waterfall::ChangeColsRows(int i, int& col, int& row)
{
	/*switch (i)
	{
	case 0:
		col = 12;
		row = 5;
		break;
	case 1:
	case 2:
		col = 5;
		row = 6;
		break;
	case 3:
		col = 2;
		row = 8;
		break;
	case 4:
		row = 9;
		break;
	case 5:
		col = 3;
		row = 14;
		break;
	case 6:
		col = 2;
		row = 8;
		break;
	case 7:
		col = 3;
		row = 6;
		break;
	case 8:
		col = 4;
		row = 8;
		break;
	case 9:
		col = 12;
		row = 6;
		break;
	case 10:
		col = 3;
		row = 6;
		break;
	case 11:
		col = 13;
		break;
	default:
		col = 0;
		row = 0;
		break;
	}*/
}